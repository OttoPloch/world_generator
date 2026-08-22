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

    decorationAtlas = game->getAssetManager()->getTextureAtlas("foliage_better");
}

void ChunkGenerator::generate(sf::Vector2i chunkPosition)
{
    generateChunk(chunkPosition);
    generateDecorations((*chunks)[chunkPosition].get());
}

void ChunkGenerator::generateChunk(sf::Vector2i chunkPosition)
{
    float tileSize = game->getSettings()->tile_size;
    int chunkSize = game->getSettings()->chunk_size;
    float chunkLength = tileSize * chunkSize;
    
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
}

void ChunkGenerator::generateDecorations(Chunk* chunk)
{
    float genFrequency = game->getSettings()->generation_decoration_frequency;
    float scale = game->getSettings()->generation_decoration_scale;
    // if the scale is 0 or less, use the scale of the tiles. This makes decorations always have the same individual pixel size as tiles.
    if (scale <= 0) scale = getTileScale(game);
    // TODO: don't hardcode tile atlas
    // makes decorations line up with other pixels    
    float positionRounding = scale;
    
    // TEMP, TODO: add a way to know what tile types a decoration is valid on? Only have grass right now,
    // but maybe i'll add water or rock decorations?
    std::vector<TileType>decorationValidTileTypes = {TileType::GRASS};
    
    sf::Vector2f decorationLocalBottom;
    sf::FloatRect decorationTexCoords;
    sf::Vector2f decorationSize;
    sf::Vector2f decorationGlobalTopleft;
    std::vector<BackgroundObject> decorations;

    for (int i = 0; i < genFrequency; i++)
    {
        createDecoration(chunk->worldPosition, scale, positionRounding, decorationLocalBottom, decorationTexCoords, decorationSize, decorationGlobalTopleft);

        if (!decorationTileTypeCheck(chunk, decorationValidTileTypes, decorationGlobalTopleft, decorationSize)) continue;

        BackgroundObject decoration = {
            {
                decorationGlobalTopleft,
                decorationSize
            },
            decorationTexCoords
        };
        
        decorations.push_back(decoration);
    }

    chunk->bgObjects = decorations;
}

void ChunkGenerator::createDecoration(sf::Vector2f& chunkWorldPosition, float& scale, float& positionRounding, sf::Vector2f& decorationLocalBottom, sf::FloatRect& decorationTexCoords, sf::Vector2f& decorationSize, sf::Vector2f& decorationGlobalTopleft)
{
    float tileSize = game->getSettings()->tile_size;
    int chunkSize = game->getSettings()->chunk_size;
    float chunkLength = tileSize * chunkSize;
    
    decorationLocalBottom = sf::Vector2f(game->random.getRandInt(1, chunkLength - 1), game->random.getRandInt(1, chunkLength - 1));

    decorationTexCoords = getRandomTextureAtlasChoice(game, decorationAtlas);
    decorationSize = {decorationTexCoords.size.x * scale, decorationTexCoords.size.y * scale};
    decorationGlobalTopleft = {
        chunkWorldPosition.x + decorationLocalBottom.x - decorationSize.x / 2.f,
        chunkWorldPosition.y + decorationLocalBottom.y - decorationSize.y
    };

    // makes decorations line up with other pixels
    decorationGlobalTopleft = {roundToMultiple(decorationGlobalTopleft.x, positionRounding), roundToMultiple(decorationGlobalTopleft.y, positionRounding)};
    // offsets the decoration to avoid stitching.
    decorationGlobalTopleft += {toFloat(game->random.getRandInt(-100000, 100000)) / 10000000.f, toFloat(game->random.getRandInt(-100000, 100000)) / 10000000.f};
}

bool ChunkGenerator::decorationTileTypeCheck(Chunk* chunk, std::vector<TileType> validTypes, sf::Vector2f decorationGlobalTopleft, sf::Vector2f decorationSize)
{
    float tileSize = game->getSettings()->tile_size;
    int chunkSize = game->getSettings()->chunk_size;

    sf::Vector2i decorationTileTopleft(worldToTilePosition(game, decorationGlobalTopleft, false));
    sf::Vector2i decorationTileBottomright(worldToTilePosition(game, decorationGlobalTopleft + decorationSize, false));

    sf::Vector2i decorationTileSize = sf::Vector2i(decorationTileBottomright - decorationTileTopleft) + sf::Vector2i(1, 1);

    sf::Vector2i currTilePos;
    bool allPositionsValid = true;
    std::vector<Tile*> decorationTiles;
    for (int y = 0; y < decorationTileSize.y; y++)
    {
        for (int x = 0; x < decorationTileSize.x; x++)
        {
            currTilePos = worldToTilePosition(game, decorationGlobalTopleft + sf::Vector2f(tileSize * x, tileSize * y), false);

            sf::Vector2i chunkGlobalTilePos(worldToChunkPosition(game, decorationGlobalTopleft));
            chunkGlobalTilePos = {chunkGlobalTilePos.x *= chunkSize, chunkGlobalTilePos.y *= chunkSize};
            // Cannot use localPos option in worldToTilePosition conversions because those will wrap around, and we
            // need to know if the local tile position is outside of the chunk by getting negative or too large values.
            sf::Vector2i localPos(currTilePos - chunkGlobalTilePos);

            if (localPos.x < 0 || localPos.x > chunkSize - 1 || localPos.y < 0 || localPos.y > chunkSize - 1)
            {
                allPositionsValid = false;
                break;
            }

            decorationTiles.emplace_back(chunk->getTile(localPos));
        }

        if (!allPositionsValid) break;
    }

    if (!allPositionsValid) return false;

    bool allTilesValid = true;
    for (auto t : decorationTiles)
    {
        if (!t)
        {
            std::cout << "ERROR in ChunkGenerator::decorationTileTypeCheck(), decoration is generating on a tile that does not exist. Chunk position: " << chunk->getChunkPosition().x << ", " << chunk->getChunkPosition().y << ".\n";
            allTilesValid = false;
            break;
        }

        bool tileIsValidType = false;
        for (auto type : validTypes)
        {
            if (t->type == type)
            {
                tileIsValidType = true;
                break;
            }
        }

        if (!tileIsValidType)
        {
            allTilesValid = false;
            break;
        }
    }

    if (!allTilesValid) return false;

    return true;
}