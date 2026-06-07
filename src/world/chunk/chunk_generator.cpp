#include "chunk_generator.hpp"
#include "../../core/game.hpp"
#include "../../entities/entity_layer.hpp"
#include "../../utils/FastNoiseLite.h"
#include "../../graphics/texture_atlas.hpp"
#include "../tile/tile_types.hpp"
#include "../../utils/utils.hpp"
#include "../../graphics/vertex_group.hpp"
#include "../background_object.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <algorithm>
#include <cmath>
#include <vector>

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

    if (game->getSettings()->worldSeed != -1)
    {
        noise.SetSeed(game->getSettings()->worldSeed);
    }
    else
    {
        noise.SetSeed(game->random.getRandInt());
    }
}

void ChunkGenerator::generate(sf::Vector2i chunkPosition)
{
    int chunkSize = game->getSettings()->chunk_size;
    float tileSize = game->getSettings()->tile_size;
    float chunkLength = toFloat(chunkSize) * tileSize;
    
    std::vector<std::vector<TileTemplate>> tileData(game->getSettings()->maxTileZ + 1);

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

    (*chunks)[chunkPosition] = std::make_unique<Chunk>(game, game->getScene()->getChunkLayer(), chunkPosition, std::move(tileData));

    // decorations
    Chunk* chunk = (*chunks)[chunkPosition].get();
    sf::Vector2f chunkWorldPos(chunkPosition.x * chunkLength, chunkPosition.y * chunkLength);
    sf::Vector2i chunkGlobalTilePos(worldToTilePosition(game, chunkToWorldPosition(game, chunkPosition)));

    sf::Texture* decorationTexture = game->getAssetManager()->getTexture("foliage_better", "texture_atlases/");
    TextureAtlas* decorationAtlas = game->getAssetManager()->getTextureAtlas("foliage_better");
    sf::FloatRect decorationTexCoords;
    float scale = game->getSettings()->generation_decoration_scale;
    if (scale < 0) scale = game->getSettings()->tile_size / game->getAssetManager()->getTextureAtlas("tiles_better")->tileSize;
    // TODO: don't hardcode tile atlas
    // makes decorations line up with other pixels    
    float positionRounding = game->getSettings()->tile_size * (1.f / game->getAssetManager()->getTextureAtlas("tiles_better")->tileSize);

    std::vector<BackgroundObject> decorations;

    TileType currDecTileType = TileType::GRASS;
    while (currDecTileType != TileType::COUNT)
    {
        for (int i = 0; i < game->getSettings()->generation_decoration_frequency; i++)
        {
            sf::Vector2f decorationBottom(game->random.getRandInt(1, chunkLength - 1), game->random.getRandInt(1, chunkLength - 1));
            // makes decorations line up with other pixels
            decorationBottom = {roundToMultiple(decorationBottom.x, positionRounding), roundToMultiple(decorationBottom.y, positionRounding)};
            // offsets the bottom to avoid stitching.
            decorationBottom += {toFloat(game->random.getRandInt(-100, 100)) / 10000.f, toFloat(game->random.getRandInt(-100, 100)) / 10000.f};

            // setting tex coords from the options given in the atlas
            std::vector<sf::FloatRect> decorationOptions;            
            for (auto t : decorationAtlas->itemTexCoords)
            {
                decorationOptions.emplace_back(t.second);
            }
            decorationTexCoords = decorationOptions[game->random.getRandInt(0, decorationOptions.size() - 1)];
            
            sf::Vector2f decorationSize(decorationTexCoords.size.x * scale, decorationTexCoords.size.y * scale);
            sf::Vector2f decorationWorldTopLeft(chunkWorldPos.x + decorationBottom.x - decorationSize.x / 2.f, chunkWorldPos.y + decorationBottom.y - decorationSize.y);

            sf::Vector2i decorationTileTopLeft(worldToTilePosition(game, decorationWorldTopLeft));
            sf::Vector2i decorationTileBottomRight(worldToTilePosition(game, decorationWorldTopLeft + decorationSize));
            // bounding box of the decoration in tileSpace
            sf::Vector2i decorationTileSize = {
                toInt(std::ceil(decorationTileBottomRight.x - decorationTileTopLeft.x) + 1),
                toInt(std::ceil(decorationTileBottomRight.y - decorationTileTopLeft.y) + 1)
            };

            // getting every tile the decoration covers
            bool allPositionsValid = true;
            std::vector<Tile*> decorationTiles;
            for (int y = 0; y < decorationTileSize.y; y++)
            {
                for (int x = 0; x < decorationTileSize.x; x++)
                {
                    sf::Vector2i pos = worldToTilePosition(game, {decorationWorldTopLeft.x + tileSize * x, decorationWorldTopLeft.y + tileSize * y});

                    // Checks if the local position of this part of the decoration is outside of the chunk
                    // (which would not be detected otherwise since the pos value is set to the remainder of pos / chunkSize,
                    // meaning that part would just get wrapped around).
                    sf::Vector2i relativePos = pos - chunkGlobalTilePos;

                    if (relativePos.x < 0 || relativePos.x > chunkSize - 1 || relativePos.y < 0 || relativePos.y > chunkSize - 1)
                    {
                        allPositionsValid = false;
                        break;
                    }

                    pos.x = pos.x % chunkSize;
                    pos.y = pos.y % chunkSize;
                    
                    decorationTiles.emplace_back(chunk->getTile(pos.x, pos.y));
                }

                if (!allPositionsValid) break;
            }

            if (!allPositionsValid) continue;


            // if (currDecTileType == TileType::WATER) std::cout << "water\n";
            // if (currDecTileType == TileType::GRASS) std::cout << "grass\n";

            // making sure all covered tiles exist and are the correct type
            bool allTilesValid = true;
            for (auto t : decorationTiles)
            {
                if (!t || t->type != currDecTileType)
                {
                    if (!t)
                    {
                        std::cout << "NONEXISTANT TILE FOR DECORATION WITH TOP LEFT AT " << decorationTileTopLeft.x << ", " << decorationTileTopLeft.y << " TILE POS.\n";
                    }    
                    
                    allTilesValid = false;
                    break;
                }
            }    
            if (!allTilesValid) continue;


            BackgroundObject decoration = {
                {
                    decorationWorldTopLeft,
                    decorationSize
                },
                decorationTexCoords
            };
            
            decorations.push_back(decoration);
        }

        // TODO: make this decoration generation system better
        if (currDecTileType == TileType::GRASS) currDecTileType = TileType::COUNT;
    }

    chunk->bgObjects = decorations;
}