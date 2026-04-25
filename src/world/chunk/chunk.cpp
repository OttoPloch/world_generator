#include "chunk.hpp"
#include "../../core/game.hpp"
#include "../tile/tile_types.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <memory>

Chunk::Chunk() {}

Chunk::Chunk(Game* game, sf::Vector2i chunkPosition, std::vector<std::vector<Tile>> tiles)
{
    this->game = game;

    this->window = game->getWindow();

    this->chunkPosition = chunkPosition;

    chunkSize = game->getSettings()->chunk_size;

    tileSize = game->getSettings()->tile_size;

    worldPosition = {chunkPosition.x * (chunkSize * tileSize), chunkPosition.y * (chunkSize * tileSize)};

    this->tiles.resize(tiles.size());
    tileVertices.resize(this->tiles.size() * chunkSize * chunkSize * 6);
    tileDebugVertices.resize(this->tiles.size());

    for (int i = 0; i < this->tiles.size(); i++)
    {
        this->tiles[i].resize(chunkSize * chunkSize);
    }

    for (int i = 0; i < this->tiles.size(); i++)
    {
        for (int j = 0; j < this->tiles[i].size(); j++)
        {
            if (tiles[i].size() > 0)
            {
                Tile* currTile = &tiles[i][j % tiles[i].size()];
        
                sf::Vector2i localPos(j % chunkSize, toInt(std::floor(j / chunkSize)));

                this->tiles[i][j] = std::make_unique<Tile>(game, this, localPos, currTile->type, currTile->myVerts.texCoords, i, currTile->collides, currTile->colliderName, currTile->collOffsetFraction, currTile->collSizeFraction);

                if (currTile->collides)
                {
                    tilesWithColliders.push_back(this->tiles[i][j].get());
                }
            }
            else
            {
                // TODO: fix hardcoded texture atlas
                this->tiles[i][j] = std::make_unique<Tile>(game, this, sf::Vector2i(j % chunkSize, toInt(std::floor(j / chunkSize))), TileType::AIR, game->getAssetManager()->getTextureAtlas("tiles_better")->getItemTexCoords("air"), 0);
            }
            
            // TEMP, TODO: find a better place for this
            if (this->tiles[i][j]->type == TileType::WATER)
            {
                this->tiles[i][j]->animation = game->getAssetManager()->getGlobalAnimation("water");
                this->tiles[i][j]->animation->animation.adjustSpeed(1.5f);
            }
            else if (this->tiles[i][j]->type == TileType::GRASS)
            {
                this->tiles[i][j]->animation = game->getAssetManager()->getGlobalAnimation("grass");
                this->tiles[i][j]->animation->animation.adjustSpeed(6);
            }
            else if (this->tiles[i][j]->type == TileType::LAVA)
            {
                this->tiles[i][j]->animation = game->getAssetManager()->getGlobalAnimation("lava");
                this->tiles[i][j]->animation->animation.adjustSpeed(.75f);
            }

            createTileVerts(j, i);
        }
    }

    state = ChunkState::ASLEEP;

    tileStates.texture = game->getAssetManager()->getTexture("tiles_better", "texture_atlases/");
}

void Chunk::createTileVerts(int index, int z)
{
    sf::Vector2f tileWorldPos = {worldPosition.x + toFloat(tiles[z][index]->localPosition.x) * tileSize, worldPosition.y + toFloat(tiles[z][index]->localPosition.y) * tileSize};
    
    sf::IntRect texCoords = tiles[z][index]->myVerts.texCoords;

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

Tile* Chunk::getTile(int column, int row, int z)
{
    int x = column;
    int y = row;

    while (x > chunkSize - 1) x -= chunkSize;
    while (x < 0) x += chunkSize;
    while (y > chunkSize - 1) y -= chunkSize;
    while (y < 0) y += chunkSize;

    int effectiveZ = z;

    int maxZ = game->getSettings()->maxTileZ;
    while (effectiveZ > maxZ) effectiveZ -= maxZ + 1;
    while (effectiveZ < 0) effectiveZ += maxZ + 1;

    return tiles[effectiveZ][y * chunkSize + x].get();
}

std::vector<std::vector<std::unique_ptr<Tile>>>* Chunk::getTiles() { return &tiles; }

sf::FloatRect Chunk::getTileRect(sf::Vector2i tileLocalPosition, int z, bool returnCenterPos)
{
    Tile* tile = getTile(tileLocalPosition.x, tileLocalPosition.y, z);

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
            tiles[i][j]->update();
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
