#include "chunk_generator.hpp"
#include "../../core/game.hpp"
#include "../../entities/entity_layer.hpp"
#include "../../utils/FastNoiseLite.h"
#include "../../graphics/texture_atlas.hpp"
#include "../tile/tile_types.hpp"
#include "../../utils/utils.hpp"
#include "../../graphics/vertex_group.hpp"
#include "../background_object.hpp"
#include <algorithm>
#include <cmath>

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

    if (int seed = game->getSettings()->worldSeed != -1)
    {
        noise.SetSeed(seed);
    }
    else
    {
        int rand = getRandInt();
        noise.SetSeed(117);
    }
}

void ChunkGenerator::generate(sf::Vector2i chunkPosition, int genMode)
{
    int chunkSize = game->getSettings()->chunk_size;
    float tileSize = game->getSettings()->tile_size;
    float chunkLength = toFloat(chunkSize) * tileSize;
    
    TextureAtlas* atlas = game->getAssetManager()->getTextureAtlas("tiles_better");

    std::vector<std::vector<TileTemplate>> tileData(game->getSettings()->maxTileZ + 1);

    if (genMode == 0)
    {
        // int tileType = getRandInt(0, 3);

        // switch (tileType)
        // {
        //     case 0:
        //         tileData[0].emplace_back(TileType::WATER, atlas->getItemTexCoords("water_basic"));
        //         break;
        //     case 1:
        //         tileData[0].emplace_back(TileType::GRASS, atlas->getItemTexCoords("grass_basic"));
        //         break;
        //     case 2:
        //         tileData[0].emplace_back(TileType::LAVA, atlas->getItemTexCoords("lava_basic"));
        //         break;
        //     case 3:
        //         tileData[0].emplace_back(Tile(TileType::PINK, atlas->getItemTexCoords("pink_basic"), 0, {}, true, "tile"));
        //         break;
        // }
    }
    else if (genMode == 1)
    {
        // for (int i = 0; i < chunkSize * chunkSize; i++)
        // {
        //     int tileType = getRandInt(0, 3);

        //     switch (tileType)
        //     {
        //         case 0:
        //             tileData[0].emplace_back(TileType::WATER, atlas->getItemTexCoords("water_basic"));
        //             break;
        //         case 1:
        //             tileData[0].emplace_back(TileType::GRASS, atlas->getItemTexCoords("grass_basic"));
        //             break;
        //         case 2:
        //             tileData[0].emplace_back(TileType::LAVA, atlas->getItemTexCoords("lava_basic"));
        //             break;
        //         case 3:
        //             tileData[0].push_back(Tile(TileType::PINK, atlas->getItemTexCoords("pink_basic"), 0, {}, true, "tile"));
        //             break;
        //     }
        // }
    }
    else if (genMode == 2)
    {
        std::vector<float> noiseData = std::vector<float>(chunkSize * chunkSize);
        int index = 0;
        for (int y = 0; y < chunkSize; y++)
        {
            for (int x = 0; x < chunkSize; x++)
            {
                noiseData[index++] = (noise.GetNoise(chunkPosition.x * chunkSize + toFloat(x), chunkPosition.y * chunkSize + toFloat(y)) + 1.f) / 2.f;

                float n = noiseData[index - 1];
            }
        }

        for (int i = 0; i < chunkSize * chunkSize; i++)
        {
            std::vector<std::string> templateNames(game->getSettings()->maxTileZ + 1);
            
            // COBBLE AT SPAWN
            if (chunkPosition.x >= -1 && chunkPosition.x < 1 && chunkPosition.y >= -1 && chunkPosition.y < 1)
            {
                for (int j = 0; j < templateNames.size(); j++)
                {
                    switch (j)
                    {
                        case 0:
                            templateNames[0] = "cobble";
                            break;
                        default:
                            templateNames[j] = "air";
                            break;
                    }
                }
            }
            else
            {
                if (noiseData[i] >= .975f)
                {
                    for (int j = 0; j < templateNames.size(); j++)
                    {
                        switch (j)
                        {
                            case 0:
                                templateNames[0] = "grass";
                                break;
                            case 1:
                                templateNames[1] = "lava";
                                break;
                            default:
                                templateNames[j] = "air";
                                break;
                        }
                    }
                }
                else if (noiseData[i] >= .8f)
                {
                    for (int j = 0; j < templateNames.size(); j++)
                    {
                        switch (j)
                        {
                            case 0:
                                templateNames[0] = "grass";
                                break;
                            case 1:
                                templateNames[1] = "stone";
                                break;
                            default:
                                templateNames[j] = "air";
                                break;
                        }
                    }
                }
                else if (noiseData[i] >= .3f)
                {
                    for (int j = 0; j < templateNames.size(); j++)
                    {
                        switch (j)
                        {
                            case 0:
                                templateNames[0] = "grass";
                                break;
                            default:
                                templateNames[j] = "air";
                                break;
                        }
                    }
                }
                else
                {
                    for (int j = 0; j < templateNames.size(); j++)
                    {
                        switch (j)
                        {
                            case 0:
                                templateNames[j] = "water";
                                break;
                            default:
                                templateNames[j] = "air";
                                break;
                        }
                    }
                }
            }

            for (int j = 0; j < templateNames.size(); j++)
            {
                std::unordered_map<std::string, TileTemplate>* templates = &game->getScene()->getChunkLayer()->tManager.tileTemplates;
                
                auto entry = templates->find(templateNames[j]);

                if (entry != templates->end())
                {
                    tileData[j].emplace_back(entry->second.getCopy());
                }
                else
                {
                    tileData[j].emplace_back((*templates)["pink"].getCopy());
                }
            }
        }

        // setting conditional colliders
        for (int i = 0; i < tileData.size(); i++)
        {
            for (int j = 0; j < tileData[i].size(); j++)
            {
                if (tileData[i][j].type == TileType::WATER)
                {
                    if
                    (
                        j % chunkSize == 0 ||
                        j % chunkSize == chunkSize - 1 ||
                        j < chunkSize ||
                        j >= tileData[i].size() - chunkSize ||
                        tileData[i][j - 1].type != TileType::WATER ||
                        tileData[i][j + 1].type != TileType::WATER ||
                        tileData[i][j - chunkSize].type != TileType::WATER ||
                        tileData[i][j + chunkSize].type != TileType::WATER
                    )
                    {
                        tileData[i][j].collides = true;
                        tileData[i][j].colliderName = "water";
                    }
                }
            }
        }
    }

    (*chunks)[chunkPosition] = std::make_unique<Chunk>(game, game->getScene()->getChunkLayer(), chunkPosition, std::move(tileData));

    // decorations
    if (genMode == 2)
    {
        Chunk* chunk = (*chunks)[chunkPosition].get();
        sf::Vector2f chunkWorldPos(chunkPosition.x * chunkLength, chunkPosition.y * chunkLength);

        sf::Texture* decorationTexture = game->getAssetManager()->getTexture("background_foliage", "texture_atlases/");
        sf::IntRect decorationTexCoords;
        float scale = game->getSettings()->generation_foliage_scale;

        std::vector<BackgroundObject> decorations;

        TileType currDecTileType = TileType::GRASS;
        while (currDecTileType != TileType::COUNT)
        {
            for (int i = 0; i < 30; i++)
            {
                sf::Vector2f decorationBottom(getRandInt(1, chunkLength - 1), getRandInt(1, chunkLength - 1));
                sf::Vector2i decorationTilePos(toInt(std::floor(decorationBottom.x / tileSize)), toInt(std::floor(decorationBottom.y / tileSize)));

                Tile* decorationTile = chunk->getTile(decorationTilePos.x, decorationTilePos.y);

                if (decorationTile->type != currDecTileType) continue;

                // TODO: don't hardcode decoration texcoords
                if (decorationTile)
                {
                    switch (getRandInt(0, 2))
                    {
                        case 0:
                            decorationTexCoords = {{0, 0}, {32, 32}};
                            break;
                        case 1:
                            decorationTexCoords = {{0, 48}, {64, 16}};
                            break;
                        default:
                            decorationTexCoords = {{48, 0}, {48, 48}};
                            break;
                    }
                }

                sf::Vector2f decorationSize(decorationTexCoords.size.x * scale, decorationTexCoords.size.y * scale);
                sf::Vector2f decorationTopleft(decorationBottom.x - decorationSize.x / 2.f, decorationBottom.y - decorationSize.y);

                BackgroundObject decoration = {
                    {
                        {chunkWorldPos.x + decorationTopleft.x, chunkWorldPos.y + decorationTopleft.y},
                        decorationSize
                    },
                    decorationTexCoords
                };

                decorations.push_back(decoration);
            }

            // TODO: make this decoration generation system better
            if (currDecTileType == TileType::GRASS) currDecTileType = TileType::WATER;
            if (currDecTileType == TileType::WATER) currDecTileType = TileType::COUNT;
        }

        chunk->bgObjects = decorations;
    }
}