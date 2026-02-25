#include "chunk_generator.hpp"
#include "game.hpp"
#include "FastNoiseLite.h"
#include "background_object.hpp"
#include "tile_types.hpp"
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

    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(.02f);
    int rand = getRandInt();
    noise.SetSeed(rand);
}

void ChunkGenerator::generate(sf::Vector2i chunkPosition, int genMode)
{
    std::vector<Tile> newTiles;

    float tileSize = game->getSettings()->getSetting("tile_size").valueFloat;
    int chunkSize = game->getSettings()->getSetting("chunk_size").valueInt;

    if (genMode == 0)
    {
        int tileType = getRandInt(0, 3);

        switch (tileType)
        {
            case 0:
                newTiles = {Tile(WATER, sf::Color::Blue)};
                break;
            case 1:
                newTiles = {Tile(GRASS, sf::Color::Green)};
                break;
            case 2:
                newTiles = {Tile(LAVA, sf::Color::Red)};
                break;
            case 3:
                newTiles = {Tile(PINK, sf::Color(255, 60, 220), true, "tile")};
                break;
        }
    }
    else if (genMode == 1)
    {
        for (int i = 0; i < chunkSize * chunkSize; i++)
        {
            int tileType = getRandInt(0, 3);

            switch (tileType)
            {
                case 0:
                    newTiles.emplace_back(WATER, sf::Color::Blue);
                    break;
                case 1:
                    newTiles.emplace_back(GRASS, sf::Color::Green);
                    break;
                case 2:
                    newTiles.emplace_back(LAVA, sf::Color::Red);
                    break;
                case 3:
                    newTiles.emplace_back(PINK, sf::Color(255, 60, 220), true, "tile");
                    break;
            }
        }
    }
    else if (genMode == 2)
    {
        std::vector<float> noiseData(chunkSize * chunkSize);

        int index = 0;

        for (int y = 0; y < chunkSize; y++)
        {
            for (int x = 0; x < chunkSize; x++)
            {
                noiseData[index++] = (noise.GetNoise(chunkPosition.x * chunkSize + toFloat(x), chunkPosition.y * chunkSize + toFloat(y)) + 1.f) / 2.f;
            }
        }

        for (int i = 0; i < noiseData.size(); i++)
        {
            int tileType;
            sf::Color tileColor;

            sf::Vector2i tilePosition = {i % chunkSize, toInt(std::floor(i / chunkSize))};

            if (getDistance(toV2F(0, 0), toV2F(chunkPosition.x * chunkSize + tilePosition.x, chunkPosition.y * chunkSize + tilePosition.y)) < 10)
            {
                // newTiles.emplace_back(4, sf::Color(110, 60, 30));
                newTiles.emplace_back(4, sf::Color(20, 20, 20));
            }
            else
            {
                if (noiseData[i] >= .92f) { newTiles.emplace_back(LAVA, sf::Color::Red); }
                else if (noiseData[i] >= .8f) { newTiles.emplace_back(ROCK, sf::Color(150, 150, 150), true, "rock"); }
                else if (noiseData[i] >= .3f) { newTiles.emplace_back(GRASS, sf::Color::Green); }
                else if (noiseData[i] - .3f > -.1f) { newTiles.emplace_back(WATER, sf::Color::Blue, true, "water"); }
                else { newTiles.emplace_back(WATER, sf::Color::Blue); }
            }
        }
    }

    (*chunks)[chunkPosition] = std::make_unique<Chunk>(game, chunkPosition, newTiles);

    if (genMode == 2)
    {
        Chunk* chunk = (*chunks)[chunkPosition].get();

        std::vector<BackgroundObject> bgObjects;

        for (int i = 0; i < 50; i++)
        {
            sf::Vector2f objCenter(getRandInt(0, chunkSize * tileSize), getRandInt(0, chunkSize * tileSize));
            sf::Vector2f objSize(32 * 3, 24 * 3);
            sf::Vector2f objTexCoords(0, 0);
            sf::Vector2f objTexCoordDimensions(32, 24);

            if (chunk->getTile(std::floor(objCenter.x / tileSize), std::floor(objCenter.y / tileSize))->type != GRASS) continue;

            bgObjects.emplace_back(objCenter, objSize, objTexCoords, objTexCoordDimensions);
        }

        chunk->createBgObjectVerts(bgObjects);
    }
}