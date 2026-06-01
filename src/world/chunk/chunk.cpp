#include "chunk.hpp"
#include "../../core/game.hpp"
#include "../tile/tile_types.hpp"
#include "chunk_layer.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <memory>

Chunk::Chunk() {}

Chunk::Chunk(Game* game, ChunkLayer* chunkLayer, sf::Vector2i chunkPosition, std::vector<std::vector<TileTemplate>> tileData)
{
    this->game = game;
    this->chunkLayer = chunkLayer;
    window = game->getWindow();
    this->chunkPosition = chunkPosition;

    chunkSize = game->getSettings()->chunk_size;
    tileSize = game->getSettings()->tile_size;

    worldPosition = {chunkPosition.x * (chunkSize * tileSize), chunkPosition.y * (chunkSize * tileSize)};

    tiles.resize(game->getSettings()->maxTileZ + 1);
    for (int i = 0; i < tiles.size(); i++)
    {
        tiles[i].resize(chunkSize * chunkSize);
    }

    tileVertices.resize(tiles.size() * chunkSize * chunkSize * 6);
    tileDebugVertices.resize(tiles.size());

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
    sf::Vector2f tileWorldPos = {worldPosition.x + toFloat(tiles[z][index]->localPosition.x) * tileSize, worldPosition.y + toFloat(tiles[z][index]->localPosition.y) * tileSize};
    
    sf::FloatRect texCoords = tiles[z][index]->myVerts.texCoords;

    // center on bottom center to allow flexible
    // sizing for the image of the tile
    tileWorldPos.x += tileSize / 2.f;
    tileWorldPos.y += tileSize;

    float texCoordRatio = toFloat(texCoords.size.x) / toFloat(texCoords.size.y);
    sf::Vector2f tileFittedSize;
    (texCoordRatio >= 1.f) ? tileFittedSize = {tileSize * texCoordRatio, tileSize} : tileFittedSize = {tileSize, tileSize / texCoordRatio};
    sf::Vector2f tileAdjustedTl = {tileWorldPos.x - tileFittedSize.x / 2.f, tileWorldPos.y - tileFittedSize.y};

    std::array<sf::Vertex, 6> verts = VertexGroup::createTriangleVerts(tileAdjustedTl, tileFittedSize, texCoords);
    for (int i = 0; i < 6; i++)
    {
        tileVertices[(chunkSize * chunkSize * 6 * z) + (index * 6 + i)] = verts[i];
    }

    if (!tiles[z][index]->collides) return;

    // assuming tl, tr, br, bl as the first 4 vertices of the array,
    // this adds those to the debug vertex array, which is lines instead
    // of triangles.
    for (int i = 0; i < 5; i++)
    {
        sf::Vertex debugVertex;

        // goes 0, 1, 2, 3, 0 so that the tl vert
        // gets added to the beginning and end.
        debugVertex.position = verts[i % 4].position;
        debugVertex.color = sf::Color::Red;

        if (i == 0 || i == 4) // if first or last (tl), do once
        {
            tileDebugVertices[z].push_back(debugVertex);
        }
        else // otherwise, add twice
        {
            tileDebugVertices[z].push_back(debugVertex);
            tileDebugVertices[z].push_back(debugVertex);
        }
    }
}

void Chunk::createTileVerts(sf::Vector2i tilePosition, int z)
{
    createTileVerts(tilePosition.y * chunkSize + tilePosition.x, z);
}

Tile* Chunk::getTile(int column, int row, bool getHighestNonAir, int z)
{
    int x = column;
    int y = row;
    int effectiveZ = z;
    if (getHighestNonAir) effectiveZ = getHighestNonAirZ(x, y);
    else wrapPosition(x, y, effectiveZ);
    if (effectiveZ == -1) return nullptr;

    return tiles[effectiveZ][y * chunkSize + x].get();
}

void Chunk::setTile(int column, int row, TileTemplate* t, bool setHighestNonAir, int z)
{
    // Wrapping values //

    int x = column;
    int y = row;
    int effectiveZ = z;
    if (setHighestNonAir) effectiveZ = getHighestNonAirZ(x, y);
    else wrapPosition(x, y, effectiveZ);
    if (effectiveZ == -1) effectiveZ = 0;

    // // // // // // //

    if (Tile* tile = tiles[effectiveZ][y * chunkSize + x].get())
    {
        if ((tile->collides && !t->collides) || (!tile->collides && t->collides))
        {
            // old tile and new tile do not have same collider value, tilesWithColliders needs to be updated.
            
            if (tile->collides)
            {
                // old tile collides but new one doesn't, remove this tile from tilesWithColliders
                for (int i = 0; i < tilesWithColliders.size(); i++)
                {
                    if (tilesWithColliders[i] == tile)
                    {
                        tilesWithColliders.erase(tilesWithColliders.begin() + i);

                        break;
                    }
                }
            }
            else
            {
                // new tile collides but old one doesn't, add tile to tilesWithColliders
                tilesWithColliders.push_back(tile);
            }
        }

        // finally set the tile
        *tile = Tile(game, this, {x, y}, *t, effectiveZ);

        createTileVerts(y * chunkSize + x, effectiveZ);
    }
    else
    {
        // no tile created at this position yet, do it now

        tiles[effectiveZ][y * chunkSize + x] = std::make_unique<Tile>(game, this, sf::Vector2i(x, y), *t, effectiveZ);
        
        if (t->collides) tilesWithColliders.push_back(tiles[effectiveZ][y * chunkSize + x].get());

        createTileVerts(y * chunkSize + x, effectiveZ);
    }
}

void Chunk::setTile(int index, TileTemplate* t, bool setHighestNonAir, int z)
{
    setTile(index % chunkSize, toInt(std::floor(index / chunkSize)), t, setHighestNonAir, z);
}

std::vector<std::vector<std::unique_ptr<Tile>>>* Chunk::getTiles() { return &tiles; }

sf::FloatRect Chunk::getTileRect(sf::Vector2i tileLocalPosition, int z, bool returnCenterPos)
{
    Tile* tile = getTile(tileLocalPosition.x, tileLocalPosition.y, false, z);

    sf::Vector2f tileWorldPos;

    if (returnCenterPos)
    {
        tileWorldPos = {worldPosition.x + (tileLocalPosition.x + .5f) * tile->size, worldPosition.y + (tileLocalPosition.y + .5f) * tile->size};
    }
    else
    {
        tileWorldPos = {worldPosition.x + tileLocalPosition.x * tile->size, worldPosition.y + tileLocalPosition.y * tile->size};
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
    for (int i = 0; i < tiles.size(); i++)
    {
        for (int j = 0; j < tiles[i].size(); j++)
        {
            if (Tile* tile = tiles[i][j].get()) tile->update(dt);
        }
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
            for (int i = 0; i < tileDebugVertices.size(); i++)
            {
                window->getWindow().draw(tileDebugVertices[i].data(), tileDebugVertices[i].size(), sf::PrimitiveType::Lines);
            }
        }
        else
        {
            window->getWindow().draw(&tileVertices[chunkSize * chunkSize * 6 * effectiveLayerView], (chunkSize * chunkSize * 6), sf::PrimitiveType::Triangles, tileStates);
            window->getWindow().draw(tileDebugVertices[effectiveLayerView].data(), tileDebugVertices[effectiveLayerView].size(), sf::PrimitiveType::Lines);
        }
    }
    else
    {
        window->getWindow().draw(tileVertices.data(), tileVertices.size(), sf::PrimitiveType::Triangles, tileStates);
    }
}
