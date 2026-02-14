#include "chunk_layer.hpp"
#include "game.hpp"
#include <array>

ChunkLayer::ChunkLayer() : chunks(0) {}

ChunkLayer::ChunkLayer(Game* game) : chunks(0)
{
    init(game);
}

void ChunkLayer::init(Game* game)
{
    this->game = game;

    this->window = game->getWindow();

    int chunksLength = 2;

    for (int y = 0; y < chunksLength; y++)
    {
        for (int x = 0; x < chunksLength; x++)
        {
            loadChunk({-1 + x, 1 + y});
        }
    }
}

bool ChunkLayer::loadChunk(sf::Vector2i chunkPosition)
{   
    if (chunks.find(chunkPosition) == chunks.end())
    {
        chunks[chunkPosition] = Chunk(game, chunkPosition);

        return true;
    }

    return false;
}

bool ChunkLayer::unloadChunk(sf::Vector2i chunkPosition)
{
    if (chunks.find(chunkPosition) != chunks.end())
    {
        chunks.erase(chunkPosition);

        return true;
    }

    return false;
}

std::vector<std::vector<Tile>*> ChunkLayer::getSurroundingTiles(sf::Vector2f position)
{
    float chunkSize = game->getSettings()->getSetting("tile_length").valueFloat * toFloat(game->getSettings()->getSetting("chunk_length").valueInt);

    sf::Vector2i convertedPosition = {toInt(std::floor(position.x / chunkSize)), toInt(std::floor(position.y / chunkSize))};

    std::array<sf::Vector2i, 9> chunksToSearch = {
        sf::Vector2i(convertedPosition.x - 1, convertedPosition.y - 1),
        sf::Vector2i(convertedPosition.x, convertedPosition.y - 1),
        sf::Vector2i(convertedPosition.x + 1, convertedPosition.y - 1),
        sf::Vector2i(convertedPosition.x - 1, convertedPosition.y),
        convertedPosition,
        sf::Vector2i(convertedPosition.x + 1, convertedPosition.y),
        sf::Vector2i(convertedPosition.x - 1, convertedPosition.y + 1),
        sf::Vector2i(convertedPosition.x, convertedPosition.y + 1),
        sf::Vector2i(convertedPosition.x + 1, convertedPosition.y + 1)
    };

    std::vector<std::vector<Tile>*> surroundingTiles;

    for (int i = 0; i < 9; i++)
    {
        if (chunks.find(chunksToSearch[i]) != chunks.end())
        {
            surroundingTiles.push_back(chunks[chunksToSearch[i]].getTiles());
        }
    }

    return surroundingTiles;
}

void ChunkLayer::tick()
{
    for (auto& i : chunks)
    {
        i.second.tick();
    }
}

void ChunkLayer::draw()
{
    for (auto& i : chunks)
    {
        i.second.draw();
    }
}