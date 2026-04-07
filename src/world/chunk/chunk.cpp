#include "chunk.hpp"
#include "../../core/game.hpp"
#include "../tile/tile_types.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <memory>

Chunk::Chunk() {}

Chunk::Chunk(Game* game, sf::Vector2i chunkPosition, std::vector<Tile> tiles)
{
    init(game, chunkPosition, std::move(tiles));
}

void Chunk::init(Game* game, sf::Vector2i chunkPosition, std::vector<Tile> tiles)
{
    this->game = game;

    this->window = game->getWindow();

    this->chunkPosition = chunkPosition;

    chunkSize = game->getSettings()->chunk_size;

    tileSize = game->getSettings()->tile_size;

    worldPosition = {chunkPosition.x * (chunkSize * tileSize), chunkPosition.y * (chunkSize * tileSize)};

    this->tiles.resize(chunkSize * chunkSize);

    tileVertices.resize(chunkSize * chunkSize * 6);

    for (int i = 0; i < this->tiles.size(); i++)
    {
        Tile* currTile = &tiles[i % tiles.size()];

        this->tiles[i] = std::make_unique<Tile>(game, this, sf::Vector2i(i % chunkSize, toInt(std::floor(i / chunkSize))), currTile->type, currTile->myVerts.texCoords, currTile->collides, currTile->colliderName, currTile->collOffsetFraction, currTile->collSizeFraction);

        // TEMP, TODO: find a better place for this
        if (this->tiles[i]->type == TileType::WATER)
        {
            this->tiles[i]->animation = game->getAssetManager()->getGlobalAnimation("water");
            this->tiles[i]->animation->animation.adjustSpeed(1.5f);
        }

        createTileVerts(i);
    }

    state = ChunkState::ASLEEP;

    tileStates.texture = game->getAssetManager()->getTexture("tiles_better", "texture_atlases/");
}

void Chunk::createTileVerts(int index)
{
    sf::Vector2f tileWorldPos = {worldPosition.x + toFloat(tiles[index]->localPosition.x) * tileSize, worldPosition.y + toFloat(tiles[index]->localPosition.y) * tileSize};
    
    sf::IntRect texCoords = tiles[index]->myVerts.texCoords;

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
        tileVertices[index * 6 + i] = verts[i];
    }

    if (!tiles[index]->collides) return;

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
            tileDebugVertices.push_back(debugVertex);
        }
        else // otherwise, add twice
        {
            tileDebugVertices.push_back(debugVertex);
            tileDebugVertices.push_back(debugVertex);
        }
    }
}

void Chunk::createTileVerts(sf::Vector2i tilePosition)
{
    createTileVerts(tilePosition.y * chunkSize + tilePosition.x);
}

Tile* Chunk::getTile(int column, int row)
{
    int x = column;
    int y = row;

    while (x > chunkSize - 1) x -= chunkSize;
    while (x < 0) x += chunkSize;
    while (y > chunkSize - 1) y -= chunkSize;
    while (y < 0) y += chunkSize;

    return tiles[y * chunkSize + x].get();
}

std::vector<std::unique_ptr<Tile>>* Chunk::getTiles() { return &tiles; }

sf::FloatRect Chunk::getTileRect(sf::Vector2i tileLocalPosition)
{
    sf::Vector2f tileWorldPos = {worldPosition.x + tileLocalPosition.x * tileSize, worldPosition.y + tileLocalPosition.y * tileSize};

    return sf::FloatRect(tileWorldPos, {tileSize, tileSize});
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
        tiles[i]->update();
    }
}

void Chunk::draw(bool debug)
{
    window->getWindow().draw(tileVertices.data(), tileVertices.size(), sf::PrimitiveType::Triangles, tileStates);

    if (debug)
    {
        window->getWindow().draw(tileDebugVertices.data(), tileDebugVertices.size(), sf::PrimitiveType::Lines);
    }
}
