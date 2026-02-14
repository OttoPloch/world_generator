#include "chunk.hpp"
#include "game.hpp"
#include "settings.hpp"

Chunk::Chunk() {}

Chunk::Chunk(Game* game, sf::Vector2i chunkPosition)
{
    init(game, chunkPosition);
}

void Chunk::init(Game* game, sf::Vector2i chunkPosition)
{
    this->game = game;

    this->window = game->getWindow();

    this->chunkPosition = chunkPosition;

    this->chunkLength = game->getSettings()->getSetting("chunk_length").valueInt;

    tileSize = game->getSettings()->getSetting("tile_length").valueFloat;

    worldPosition = {chunkPosition.x * (chunkLength * tileSize), chunkPosition.y * (chunkLength * tileSize)};

    tiles.resize(chunkLength * chunkLength);

    tileVertices.resize(chunkLength * chunkLength * 6);

    createTiles();
}

void Chunk::createTiles()
{
    for (int i = 0; i < tiles.size(); i++)
    {
        sf::Vector2f tilePosition = {worldPosition.x + (tileSize * toFloat(i % chunkLength)), worldPosition.y + (tileSize * (std::floor(toFloat(i / chunkLength))))};
        
        int tileType = getRandInt(0, 3);

        bool tileCollides = (tileType == 3);

        tiles[i] = Tile(tilePosition, {tileSize, tileSize}, tileType, tileCollides, "tile");

        createTileVerts(i);
    }
}

void Chunk::createTileVerts(int index)
{
    sf::Vertex tl;
    sf::Vertex tr;
    sf::Vertex bl;
    sf::Vertex br;

    sf::Vector2f tilePos = tiles[index].getPosition();

    tl.position = tilePos;
    tr.position = {tilePos.x + tileSize, tilePos.y};
    bl.position = {tilePos.x, tilePos.y + tileSize};
    br.position = {tilePos.x + tileSize, tilePos.y + tileSize};

    sf::Color tileColor; 
    
    switch (tiles[index].getType())
    {
        case 0:
            tileColor = sf::Color::Green;
            break;
        case 1:
            tileColor = sf::Color::Blue;
            break;
        case 2:
            tileColor = sf::Color::Red;
            break;
        default:
            tileColor = sf::Color(255, 60, 220);
            break;
    }

    tl.color = tileColor;
    tr.color = tileColor;
    bl.color = tileColor;
    br.color = tileColor;

    tileVertices[index * 6] = tl;
    tileVertices[index * 6 + 1] = tr;
    tileVertices[index * 6 + 2] = bl;
    tileVertices[index * 6 + 3] = bl;
    tileVertices[index * 6 + 4] = tr;
    tileVertices[index * 6 + 5] = br;
}

std::vector<Tile>* Chunk::getTiles() { return &tiles; }

sf::Vector2i Chunk::getChunkPosition() { return chunkPosition; }

void Chunk::tick()
{

}

void Chunk::draw()
{
    window->getWindow().draw(&tileVertices[0], tileVertices.size(), sf::PrimitiveType::Triangles);
}
