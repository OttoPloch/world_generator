#include "chunk_generator.hpp"
#include "game.hpp"
#include "chunk_layer.hpp"

ChunkGenerator::ChunkGenerator() {}
    
ChunkGenerator::ChunkGenerator(Game* game, std::unordered_map<sf::Vector2i, Chunk, Vector2iHash>* chunks)
{
    init(game, chunks);
}

void ChunkGenerator::init(Game* game, std::unordered_map<sf::Vector2i, Chunk, Vector2iHash>* chunks)
{
    this->game = game;

    this->chunks = chunks;
}

void ChunkGenerator::generate(sf::Vector2i chunkPosition, int genMode)
{
    Chunk newChunk;

    if (genMode == 0)
    {
        float tileSize = game->getSettings()->getSetting("tile_size").valueFloat;

        int tileType = getRandInt(0, 3);

        Tile tile;

        switch (tileType)
        {
            case 3:
                tile = Tile(tileType, true, "tile");
                break;
            default:
                tile = Tile(tileType);
                break;
        }

        newChunk.init(game, chunkPosition, {tile});
    }

    (*chunks)[chunkPosition] = newChunk;
}