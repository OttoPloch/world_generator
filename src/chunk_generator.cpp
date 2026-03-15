#include "chunk_generator.hpp"
#include "entity_layer.hpp"
#include "game.hpp"
#include "FastNoiseLite.h"
#include "texture_atlas.hpp"
#include "tile_types.hpp"
#include "utils.hpp"
#include "vertex_group.hpp"
#include "background_object.hpp"
#include <algorithm>

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

    TextureAtlas* atlas = game->getAssetManager()->getTextureAtlas("tiles_better");

    int chunkSize = game->getSettings()->getSetting("chunk_size").valueInt;
    float tileSize = game->getSettings()->getSetting("tile_size").valueFloat;
    float chunkLength = toFloat(chunkSize) * tileSize;

    if (genMode == 0)
    {
        int tileType = getRandInt(0, 3);

        switch (tileType)
        {
            case 0:
                newTiles.emplace_back(TileType::WATER, atlas->getItemTexCoords("water_basic"));
                break;
            case 1:
                newTiles.emplace_back(TileType::GRASS, atlas->getItemTexCoords("grass_basic"));
                break;
            case 2:
                newTiles.emplace_back(TileType::LAVA, atlas->getItemTexCoords("lava_basic"));
                break;
            case 3:
                newTiles.emplace_back(TileType::PINK, atlas->getItemTexCoords("pink_basic"), true, "tile");
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
                    newTiles.emplace_back(TileType::WATER, atlas->getItemTexCoords("water_basic"));
                    break;
                case 1:
                    newTiles.emplace_back(TileType::GRASS, atlas->getItemTexCoords("grass_basic"));
                    break;
                case 2:
                    newTiles.emplace_back(TileType::LAVA, atlas->getItemTexCoords("lava_basic"));
                    break;
                case 3:
                    newTiles.emplace_back(TileType::PINK, atlas->getItemTexCoords("pink_basic"), true, "tile");
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

            if (getDistance({-tileSize / 2.f, -tileSize / 2.f}, {chunkPosition.x * chunkLength + tilePosition.x * tileSize, chunkPosition.y * chunkLength + tilePosition.y * tileSize}) <= tileSize * 10)
            {
                newTiles.emplace_back(TileType::COBBLE, atlas->getItemTexCoords("cobble"));
            }
            else
            {
                if (noiseData[i] >= .92f) { newTiles.emplace_back(TileType::LAVA, atlas->getItemTexCoords("lava")); }
                else if (noiseData[i] >= .8f) { newTiles.emplace_back(TileType::STONE, atlas->getItemTexCoords("stone"), true, "stone"); }
                else if (noiseData[i] >= .3f) { newTiles.emplace_back(TileType::GRASS, atlas->getItemTexCoords("grass")); }
                else if (noiseData[i] - .3f > -.1f) { newTiles.emplace_back(TileType::WATER, atlas->getItemTexCoords("water")); }
                else { newTiles.emplace_back(TileType::WATER, atlas->getItemTexCoords("water")); }
            }
        }

        // water collides if next to non-water tile
        int end = newTiles.size() - 1;
        for (int i = 0; i <= end; i++)
        {
            if (newTiles[i].type == TileType::WATER)
            {
                if (
                    i % chunkSize == 0 ||
                    i % chunkSize == chunkSize - 1 ||
                    i < chunkSize ||
                    i >= newTiles.size() - chunkSize ||
                    newTiles[i - 1].type != TileType::WATER ||
                    newTiles[i + 1].type != TileType::WATER ||
                    newTiles[i - chunkSize].type != TileType::WATER ||
                    newTiles[i + chunkSize].type != TileType::WATER)
                {
                    newTiles[i].collides = true;
                    newTiles[i].colliderName = "water";
                }
            }
        }
    }

    (*chunks)[chunkPosition] = std::make_unique<Chunk>(game, chunkPosition, std::move(newTiles));

    // decorations
    if (genMode == 2)
    {
        Chunk* chunk = (*chunks)[chunkPosition].get();

        sf::Vector2f chunkWorldPos(chunkPosition.x * chunkLength, chunkPosition.y * chunkLength);

        sf::Texture* decTexture = game->getAssetManager()->getTexture("background_foliage", "texture_atlases/");
        sf::IntRect decTexCoords;
        float scale = game->getSettings()->getSetting("generation_foliage_scale").valueFloat;

        std::vector<BackgroundObject> decorations;

        TileType currDecTileType = TileType::GRASS;
        while (currDecTileType != TileType::COUNT)
        {
            for (int i = 0; i < 30; i++)
            {
                sf::Vector2f decBottom(getRandInt(1, chunkLength - 1), getRandInt(1, chunkLength - 1));
    
                if (chunk->getTile(std::floor(decBottom.x / tileSize), std::floor(decBottom.y / tileSize))->type != currDecTileType) continue;

                switch(getRandInt(0, 2))
                {
                    case 0:
                        decTexCoords = {{0, 0}, {32, 32}};
                        break;
                    case 1:
                        decTexCoords = {{0, 48}, {64, 16}};
                        break;
                    case 2:
                        decTexCoords = {{48, 0}, {48, 48}};
                        break;
                }
                
                sf::Vector2f decSize = {decTexCoords.size.x * scale, decTexCoords.size.y * scale};
                sf::Vector2f decTl = {decBottom.x - decSize.x / 2.f, decBottom.y - decSize.y};

                BackgroundObject dec;
                dec.rect = {{chunkWorldPos.x + decTl.x, chunkWorldPos.y + decTl.y}, {decSize.x, decSize.y}};
                dec.texCoords = decTexCoords;

                decorations.push_back(dec);
            }

            if (currDecTileType == TileType::GRASS) currDecTileType = TileType::WATER;
            if (currDecTileType == TileType::WATER) currDecTileType = TileType::COUNT;
        }

        chunk->bgObjects = decorations;
    }
}