#include "chunk.hpp"
#include "game.hpp"
#include "settings.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

Chunk::Chunk() {}

Chunk::Chunk(Game* game, sf::Vector2i chunkPosition, std::vector<Tile> tiles)
{
    init(game, chunkPosition, tiles);
}

void Chunk::init(Game* game, sf::Vector2i chunkPosition, std::vector<Tile> tiles)
{
    this->game = game;

    this->window = game->getWindow();

    this->chunkPosition = chunkPosition;

    chunkSize = game->getSettings()->getSetting("chunk_size").valueInt;

    tileSize = game->getSettings()->getSetting("tile_size").valueFloat;

    worldPosition = {chunkPosition.x * (chunkSize * tileSize), chunkPosition.y * (chunkSize * tileSize)};

    this->tiles.resize(chunkSize * chunkSize);

    tileVertices.resize(chunkSize * chunkSize * 6);

    for (int i = 0; i < this->tiles.size(); i++)
    {
        Tile* currTile = &tiles[i % tiles.size()];

        this->tiles[i] = Tile(game, this, {i % chunkSize, toInt(std::floor(i / chunkSize))}, currTile->type, currTile->color, currTile->collides, currTile->colliderName, currTile->collOffsetFraction, currTile->collSizeFraction);

        createTileVerts(i);
    }
}

void Chunk::createTileVerts(int index)
{
    sf::Vertex tl;
    sf::Vertex tr;
    sf::Vertex bl;
    sf::Vertex br;

    sf::Vector2f tileWorldPos = {worldPosition.x + toFloat(tiles[index].localPosition.x) * tileSize, worldPosition.y + toFloat(tiles[index].localPosition.y) * tileSize};
    
    tl.position = tileWorldPos;
    tr.position = {tileWorldPos.x + tileSize, tileWorldPos.y};
    bl.position = {tileWorldPos.x, tileWorldPos.y + tileSize};
    br.position = {tileWorldPos.x + tileSize, tileWorldPos.y + tileSize};

    tl.color = tiles[index].color;
    tr.color = tiles[index].color;
    bl.color = tiles[index].color;
    br.color = tiles[index].color;

    tileVertices[index * 6] = tl;
    tileVertices[index * 6 + 1] = tr;
    tileVertices[index * 6 + 2] = bl;
    tileVertices[index * 6 + 3] = bl;
    tileVertices[index * 6 + 4] = tr;
    tileVertices[index * 6 + 5] = br;
}

std::vector<Tile>* Chunk::getTiles() { return &tiles; }

sf::FloatRect Chunk::getTileRect(sf::Vector2i tileLocalPosition)
{
    sf::Vector2f tileWorldPos = {worldPosition.x + tileLocalPosition.x * tileSize, worldPosition.y + tileLocalPosition.y * tileSize};

    return sf::FloatRect(tileWorldPos, {tileSize, tileSize});
}

sf::Vector2i Chunk::getChunkPosition() { return chunkPosition; }

void Chunk::tick()
{

}

void Chunk::draw(bool debug)
{
    window->getWindow().draw(&tileVertices[0], tileVertices.size(), sf::PrimitiveType::Triangles);

    if (debug)
    {
        for (int i = 0; i < tiles.size(); i++)
        {
            if (tiles[i].collides)
            {
                sf::RectangleShape rect(tiles[i].getCollRect().size);

                rect.setPosition(tiles[i].getCollRect().position);
                rect.setFillColor(sf::Color::Transparent);
                rect.setOutlineColor(sf::Color::Red);
                rect.setOutlineThickness(10.f);

                window->draw(rect);
            }
        }
    }
}
