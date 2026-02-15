#include "chunk_generator.hpp"
#include "game.hpp"
#include "chunk_layer.hpp"
#include "utils.hpp"

ChunkGenerator::ChunkGenerator() {}
    
ChunkGenerator::ChunkGenerator(Game* game, std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash>* chunks)
{
    init(game, chunks);
}

void ChunkGenerator::init(Game* game, std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash>* chunks)
{
    this->game = game;

    this->chunks = chunks;
}

void ChunkGenerator::generate(sf::Vector2i chunkPosition, int genMode)
{
    std::vector<Tile> newTiles;

    float tileSize = game->getSettings()->getSetting("tile_size").valueFloat;
    int chunkSize = game->getSettings()->getSetting("chunk_size").valueInt;

    if (genMode == 0)
    {
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

        newTiles = {tile};
    }
    else if (genMode == 1)
    {
        for (int i = 0; i < chunkSize * chunkSize; i++)
        {
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

            newTiles.push_back(tile);
        }
    }

    (*chunks)[chunkPosition] = std::make_unique<Chunk>(game, chunkPosition, newTiles);
}