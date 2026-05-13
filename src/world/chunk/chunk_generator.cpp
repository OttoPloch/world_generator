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

    std::vector<std::vector<TileTemplate>> newTiles(game->getSettings()->maxTileZ + 1);

    if (genMode == 0)
    {
        // int tileType = getRandInt(0, 3);

        // switch (tileType)
        // {
        //     case 0:
        //         newTiles[0].emplace_back(TileType::WATER, atlas->getItemTexCoords("water_basic"));
        //         break;
        //     case 1:
        //         newTiles[0].emplace_back(TileType::GRASS, atlas->getItemTexCoords("grass_basic"));
        //         break;
        //     case 2:
        //         newTiles[0].emplace_back(TileType::LAVA, atlas->getItemTexCoords("lava_basic"));
        //         break;
        //     case 3:
        //         newTiles[0].emplace_back(Tile(TileType::PINK, atlas->getItemTexCoords("pink_basic"), 0, {}, true, "tile"));
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
        //             newTiles[0].emplace_back(TileType::WATER, atlas->getItemTexCoords("water_basic"));
        //             break;
        //         case 1:
        //             newTiles[0].emplace_back(TileType::GRASS, atlas->getItemTexCoords("grass_basic"));
        //             break;
        //         case 2:
        //             newTiles[0].emplace_back(TileType::LAVA, atlas->getItemTexCoords("lava_basic"));
        //             break;
        //         case 3:
        //             newTiles[0].push_back(Tile(TileType::PINK, atlas->getItemTexCoords("pink_basic"), 0, {}, true, "tile"));
        //             break;
        //     }
        // }
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

        for (int i = 0; i < chunkSize * chunkSize; i++)
        {
            std::vector<std::string> tileData(game->getSettings()->maxTileZ + 1);
            
            // COBBLE AT SPAWN
            if (chunkPosition.x >= -1 && chunkPosition.x < 1 && chunkPosition.y >= -1 && chunkPosition.y < 1)
            {
                for (int j = 0; j < tileData.size(); j++)
                {
                    switch (j)
                    {
                        case 0:
                            tileData[0] = "cobble";
                            break;
                        default:
                            tileData[j] = "air";
                            break;
                    }
                }
            }
            else
            {
                if (noiseData[i] >= .975f)
                {
                    for (int j = 0; j < tileData.size(); j++)
                    {
                        switch (j)
                        {
                            case 0:
                                tileData[0] = "grass";
                                break;
                            case 1:
                                tileData[1] = "lava";
                                break;
                            default:
                                tileData[j] = "air";
                                break;
                        }
                    }
                }
                else if (noiseData[i] >= .8f)
                {
                    for (int j = 0; j < tileData.size(); j++)
                    {
                        switch (j)
                        {
                            case 0:
                                tileData[0] = "grass";
                                break;
                            case 1:
                                tileData[1] = "stone";
                                break;
                            default:
                                tileData[j] = "air";
                                break;
                        }
                    }
                }
                else if (noiseData[i] >= .3f)
                {
                    for (int j = 0; j < tileData.size(); j++)
                    {
                        switch (j)
                        {
                            case 0:
                                tileData[0] = "grass";
                                break;
                            default:
                                tileData[j] = "air";
                                break;
                        }
                    }
                }
                else
                {
                    for (int j = 0; j < tileData.size(); j++)
                    {
                        switch (j)
                        {
                            case 0:
                                tileData[j] = "water";
                                break;
                            default:
                                tileData[j] = "air";
                                break;
                        }
                    }
                }
            }

            for (int j = 0; j < tileData.size(); j++)
            {
                if (Chunk* chunk = (*chunks)[chunkPosition].get())
                {
                    std::unordered_map<std::string, TileTemplate>* templates = &(*chunks)[chunkPosition]->chunkLayer->tManager.tileTemplates;
                    
                    auto entry = templates->find(tileData[j]);
    
                    if (entry != templates->end())
                    {
                        newTiles[j].emplace_back(std::move(entry->second));
                    }
                    else
                    {
                        newTiles[j].emplace_back(std::move((*templates)["pink"]));
                    }
                }
            }
        }

        // setting colliders
        for (int i = 0; i < newTiles.size(); i++)
        {
            for (int j = 0; j < newTiles[i].size(); j++)
            {
                if (newTiles[i][j].type == TileType::WATER)
                {
                    if
                    (
                        j % chunkSize == 0 ||
                        j % chunkSize == chunkSize - 1 ||
                        j < chunkSize ||
                        j >= newTiles[i].size() - chunkSize ||
                        newTiles[i][j - 1].type != TileType::WATER ||
                        newTiles[i][j + 1].type != TileType::WATER ||
                        newTiles[i][j - chunkSize].type != TileType::WATER ||
                        newTiles[i][j + chunkSize].type != TileType::WATER
                    )
                    {
                        newTiles[i][j].collides = true;
                        newTiles[i][j].colliderName = "water";
                    }
                }

                if (newTiles[i][j].type == TileType::STONE)
                {
                    newTiles[i][j].collides = true;
                }
            }
        }
    }

    (*chunks)[chunkPosition] = std::make_unique<Chunk>(game, game->getScene()->getChunkLayer(), chunkPosition, std::move(newTiles));

    // decorations
    if (genMode == 2)
    {
        Chunk* chunk = (*chunks)[chunkPosition].get();

        sf::Vector2f chunkWorldPos(chunkPosition.x * chunkLength, chunkPosition.y * chunkLength);

        sf::Texture* decTexture = game->getAssetManager()->getTexture("background_foliage", "texture_atlases/");
        sf::IntRect decTexCoords;
        float scale = game->getSettings()->generation_foliage_scale;

        std::vector<BackgroundObject> decorations;

        TileType currDecTileType = TileType::GRASS;
        while (currDecTileType != TileType::COUNT)
        {
            for (int i = 0; i < 30; i++)
            {
                sf::Vector2f decBottom(getRandInt(1, chunkLength - 1), getRandInt(1, chunkLength - 1));

                // TODO: right now decorations only check the top layer of the chunk when deciding to generate. This is most likely fine
                // and maybe the best solution, but I'm not sure if I will want to keep it this way forever.
                int decCheckTileZ = -1;
                TileType currZTileType = chunk->getTile(std::floor(decBottom.x / tileSize), std::floor(decBottom.y / tileSize), decCheckTileZ)->type;
                while (currZTileType == TileType::AIR)
                {
                    decCheckTileZ--;
                    currZTileType = chunk->getTile(std::floor(decBottom.x / tileSize), std::floor(decBottom.y / tileSize), decCheckTileZ)->type;
                }
                if (currZTileType != currDecTileType) continue;

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

            // TODO: make this decoration generation system better
            if (currDecTileType == TileType::GRASS) currDecTileType = TileType::WATER;
            if (currDecTileType == TileType::WATER) currDecTileType = TileType::COUNT;
        }

        chunk->bgObjects = decorations;
    }
}