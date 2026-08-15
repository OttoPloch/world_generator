#include "chunk.hpp"
#include "../../core/game.hpp"
#include "../tile/tile_types.hpp"
#include "chunk_layer.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <memory>
#include <unordered_map>
#include <algorithm>

Chunk::Chunk() {}

Chunk::Chunk(Game* game, ChunkLayer* chunkLayer, sf::Vector2i chunkPosition, std::vector<std::vector<TileTemplate>> tileData)
{
    this->game = game;
    this->chunkLayer = chunkLayer;
    window = game->getWindow();
    this->chunkPosition = chunkPosition;

    tileSize = game->getSettings()->tile_size;
    chunkSize = game->getSettings()->chunk_size;
    float chunkLength = tileSize * chunkSize;

    sf::Vector2i worldOrigin(game->getScene()->getWorldChunkOrigin());
    worldPosition = {(chunkPosition.x - worldOrigin.x) * chunkLength, (chunkPosition.y - worldOrigin.y) * chunkLength};

    tiles.resize(game->getSettings()->maxTileZ + 1);
    for (int i = 0; i < tiles.size(); i++)
    {
        tiles[i].resize(chunkSize * chunkSize);
    }

    // (z layers) * (tiles in a layer) * (6 vertices per tile)
    tileVertices.resize(tiles.size() * chunkSize * chunkSize * 6);
    
    tileDebugVertices.resize(tiles.size() * chunkSize * chunkSize * 8); // 8 vertices per tile to draw line rects not triangles

    for (int i = 0; i < tiles.size(); i++)
    {
        for (int j = 0; j < tiles[i].size(); j++)
        {
            if (tileData[i].size() > 0) // if the tileData is not empty, use it
            {
                setTile(j, &tileData[i][j % tileData[i].size()], false, i);
            }
            else // tileData has no data, just do all air
            {
                setTile(j, &chunkLayer->tManager.tileTemplates["air"], false, i);
            }
        }
    }

    state = ChunkState::ASLEEP;

    tileStates.texture = game->getAssetManager()->getTexture("tiles_better", "texture_atlases/");
}

void Chunk::createTileVerts(int index, int z)
{
    Tile* tile = getTile(index, false, z);
    if (!tile) return;

    sf::Vector2f tilePosition = {worldPosition.x + tile->localPosition.x * tileSize, worldPosition.y + tile->localPosition.y * tileSize};
    sf::FloatRect texCoords = tile->myVerts.texCoords;
    
    sf::Vector2f tileWorldSize = {tileSize, tileSize}; // just here so it can be set to 0, 0 if the tile is not meant to be visible
    if (texCoords.size == sf::Vector2f(0, 0)) tileWorldSize = {0, 0};

    std::array<sf::Vertex, 6> verts = VertexGroup::createTriangleVerts(tilePosition, tileWorldSize, texCoords);
    for (int i = 0; i < 6; i++)
    {
        tileVertices[(chunkSize * chunkSize * 6 * z) + (index * 6) + i] = verts[i];
    }

    

    if (tile->collides) // has a collider, set verts
    {
        std::array<sf::Vertex, 8> colliderVerts = VertexGroup::createLineVerts(tilePosition, {tileSize, tileSize}, sf::Color::Red);
        for (int i = 0; i < 8; i++)
        {
            tileDebugVertices[(chunkSize * chunkSize * 8 * z) + (index * 8) + i] = colliderVerts[i];
        }
    }
    else // no collider, erase verts
    {
        std::array<sf::Vertex, 8> colliderVerts = VertexGroup::createLineVerts(tilePosition, {0, 0}, sf::Color(0, 0, 0, 0));
        for (int i = 0; i < 8; i++)
        {
            tileDebugVertices[(chunkSize * chunkSize * 8 * z) + (index * 8) + i] = colliderVerts[i];
        }
    }
}

void Chunk::createTileVerts(sf::Vector2i localPosition, int z)
{
    createTileVerts(localPosition.y * chunkSize + localPosition.x, z);
}

void Chunk::createAllTileVerts()
{
    for (int i = 0; i < tiles.size(); i++)
    {
        for (int j = 0; j < tiles[i].size(); j++)
        {
            createTileVerts(j, i);
        }
    }
}

Tile* Chunk::getTile(sf::Vector2i localPosition, bool getHighestNonAir, int z)
{
    int x = localPosition.x;
    int y = localPosition.y;
    int effectiveZ = z;
    if (getHighestNonAir) effectiveZ = getHighestNonAirZ(x, y);
    else wrapPosition(x, y, effectiveZ);
    if (effectiveZ == -1) return nullptr;

    return tiles[effectiveZ][y * chunkSize + x].get();
}

Tile* Chunk::getTile(int index, bool getHighestNonAir, int z)
{
    return getTile({index % chunkSize, static_cast<int>(index / chunkSize)}, getHighestNonAir, z);
}

void Chunk::setTile(sf::Vector2i localPositon, TileTemplate* t, bool setHighestNonAir, int z)
{
    // Wrapping values //

    int x = localPositon.x;
    int y = localPositon.y;
    int effectiveZ = z;
    if (setHighestNonAir) effectiveZ = getHighestNonAirZ(x, y);
    else wrapPosition(x, y, effectiveZ);
    if (effectiveZ == -1) effectiveZ = 0;

    // // // // // // //

    Tile* tile = tiles[effectiveZ][y * chunkSize + x].get();
    
    if (tile) // replacing an old tile
    {
        if (t->collides)
        {
            if (!tile->collides) tilesWithColliders.push_back(tile);
        }
        else
        {
            if (tile->collides)
            {
                auto i = std::find(tilesWithColliders.begin(), tilesWithColliders.end(), tile);
                if (i != tilesWithColliders.end()) tilesWithColliders.erase(i);
                else std::cout << "TILE WITH COLLIDER NOT FOUND IN tilesWithColliders VECTOR IN CHUNK AT " << chunkPosition.x << ", " << chunkPosition.y << ". TILE POSITION IS " << tile->localPosition.x << ", " << tile->localPosition.y << " AND TYPE IS " << static_cast<int>(tile->type) << ".\n";
            }
        }

        bool oldHasAnimation = (tile->animation || tile->globalAnimation);
        bool newHasAnimation = (t->animation || t->globalAnimation);

        if (newHasAnimation)
        {
            if (!oldHasAnimation) tilesWithAnimations.push_back(tile);
        }
        else
        {
            if (oldHasAnimation)
            {
                auto i = std::find(tilesWithAnimations.begin(), tilesWithAnimations.end(), tile);
                if (i != tilesWithAnimations.end()) tilesWithAnimations.erase(i);
                else std::cout << "TILE WITH ANIMATION NOT FOUND IN tilesWithAnimations VECTOR IN CHUNK AT " << chunkPosition.x << ", " << chunkPosition.y << ". TILE POSITION IS " << tile->localPosition.x << ", " << tile->localPosition.y << " AND TYPE IS " << static_cast<int>(tile->type) << ".\n";
            }
        }

        // finally set the tile
        *tile = Tile(game, this, {x, y}, *t, effectiveZ);

        createTileVerts({x, y}, effectiveZ);
    }
    else // no tile created at this position yet, do it now
    {
        tiles[effectiveZ][y * chunkSize + x] = std::make_unique<Tile>(game, this, sf::Vector2i(x, y), *t, effectiveZ);
        
        if (t->collides) tilesWithColliders.push_back(tiles[effectiveZ][y * chunkSize + x].get());
        if (t->animation || t->globalAnimation) tilesWithAnimations.push_back(tiles[effectiveZ][y * chunkSize + x].get());

        createTileVerts({x, y}, effectiveZ);
    }
}

void Chunk::setTile(int index, TileTemplate* t, bool setHighestNonAir, int z)
{
    setTile({index % chunkSize, static_cast<int>(index / chunkSize)}, t, setHighestNonAir, z);
}

std::vector<std::vector<std::unique_ptr<Tile>>>* Chunk::getTiles() { return &tiles; }

sf::FloatRect Chunk::getTileRect(sf::Vector2i localPosition, int z, bool returnCenterPos)
{
    Tile* tile = getTile(localPosition, false, z);

    sf::Vector2f tileWorldPos;

    if (returnCenterPos)
    {
        tileWorldPos = {worldPosition.x + (localPosition.x + .5f) * tile->size, worldPosition.y + (localPosition.y + .5f) * tile->size};
    }
    else
    {
        tileWorldPos = {worldPosition.x + localPosition.x * tile->size, worldPosition.y + localPosition.y * tile->size};
    }

    return sf::FloatRect(tileWorldPos, {tile->size, tile->size});
}

std::vector<sf::Vertex>* Chunk::getVertices() { return &tileVertices; }

int Chunk::getHighestNonAirZ(int& column, int& row, bool alsoWrapPosition)
{
    if (alsoWrapPosition) wrapPosition(column, row);
    
    int maxZ = game->getSettings()->maxTileZ;
    int z = maxZ;

    if (Tile* tile = tiles[z][row * chunkSize + column].get())
    {
        while (tile->type == TileType::AIR && z > 0)
        {
            z--;
    
            tile = tiles[z][row * chunkSize + column].get();
        }

        if (tile->type == TileType::AIR) z = -1;
    }
    else
    {
        z = -1;
    }

    return z;
}

void Chunk::wrapPosition(int& column, int& row)
{
    while (column > chunkSize - 1) column -= chunkSize;
    while (column < 0) column += chunkSize;
    while (row > chunkSize - 1) row -= chunkSize;
    while (row < 0) row += chunkSize;
}

void Chunk::wrapPosition(int& column, int& row, int& z)
{
    while (column > chunkSize - 1) column -= chunkSize;
    while (column < 0) column += chunkSize;
    while (row > chunkSize - 1) row -= chunkSize;
    while (row < 0) row += chunkSize;
    
    int maxZ = game->getSettings()->maxTileZ;
    while (z > maxZ) z -= maxZ + 1;
    while (z < 0) z += maxZ + 1;
}

sf::Vector2i Chunk::getChunkPosition() { return chunkPosition; }

void Chunk::tick()
{

}

void Chunk::update(float dt)
{
    for (auto t : tilesWithAnimations)
    {
        t->update(dt);
    }
}

void Chunk::draw(bool debug, int debugLayerView)
{
    if (debug)
    {
        // the effectiveLayerView can be at -1 here, this will show all layers
        // that way you can still get the debug view without limiting the drawing
        // to only one layer.

        int effectiveLayerView = debugLayerView;
        while (effectiveLayerView > game->getSettings()->maxTileZ) effectiveLayerView -= game->getSettings()->maxTileZ + 2;
        while (effectiveLayerView < -1) effectiveLayerView += game->getSettings()->maxTileZ + 2;

        if (effectiveLayerView == -1)
        {
            window->getWindow().draw(tileVertices.data(), tileVertices.size(), sf::PrimitiveType::Triangles, tileStates);
            window->getWindow().draw(tileDebugVertices.data(), tileDebugVertices.size(), sf::PrimitiveType::Lines);
        }
        else
        {
            window->getWindow().draw(&tileVertices[chunkSize * chunkSize * 6 * effectiveLayerView], (chunkSize * chunkSize * 6), sf::PrimitiveType::Triangles, tileStates);
            window->getWindow().draw(&tileDebugVertices[chunkSize * chunkSize * 8 * effectiveLayerView], (chunkSize * chunkSize * 8), sf::PrimitiveType::Lines);
        }
    }
    else
    {
        window->getWindow().draw(tileVertices.data(), tileVertices.size(), sf::PrimitiveType::Triangles, tileStates);
    }
}
