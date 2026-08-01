#include "chunk_layer.hpp"
#include "../../core/game.hpp"
#include "../../utils/utils.hpp"
#include "../background_object.hpp"
#include "../../entities/entity_layer.hpp"
#include "../../graphics/vertex_group.hpp"
#include "chunk.hpp"
#include "chunk_state.hpp"
#include "../../entities/components/position_component.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <algorithm>
#include <functional>
#include <memory>

ChunkLayer::ChunkLayer() : chunks(0) {}

ChunkLayer::ChunkLayer(Game* game) : chunks(0)
{
    init(game);
}

void ChunkLayer::init(Game* game)
{
    this->game = game;
    this->window = game->getWindow();

    chunkSize = game->getSettings()->chunk_size;
    tileSize = game->getSettings()->tile_size;
    chunkLength = toFloat(chunkSize) * tileSize;
    lastChunkPos = {INT32_MAX, INT32_MAX};

    chunkGenerator.init(game, &chunks);
    bgObjectStates.texture = game->getAssetManager()->getTexture("foliage_better", "texture_atlases/");


    TextureAtlas* atlas = game->getAssetManager()->getTextureAtlas("tiles_better");

    std::vector<std::unique_ptr<TileTag>> airTags = {};
    tManager.tileTemplates["air"] = {
        TileType::AIR,
        false,
        {0.f, 0.f},
        {1.f, 1.f},
        "none",
        {atlas->itemTexCoords["air"]},
        nullptr,
        nullptr,
        1.f,
        std::move(airTags)
    };

    std::vector<std::unique_ptr<TileTag>> waterTags = {};
    tManager.tileTemplates["water"] = {
        TileType::WATER,
        false, // will be set to true by ChunkGenerator if touching non-water or on chunk edge
        {0.f, 0.f},
        {1.f, 1.f},
        "none",
        {atlas->itemTexCoords["water"]},
        game->getAssetManager()->getGlobalAnimation("water"),
        nullptr,
        1.f,
        std::move(waterTags)
    };

    std::vector<std::unique_ptr<TileTag>> grassTags = {};
    tManager.tileTemplates["grass"] = {
        TileType::GRASS,
        false,
        {0.f, 0.f},
        {1.f, 1.f},
        "none",
        {atlas->itemTexCoords["grass"]},
        nullptr,
        nullptr,
        1.f,
        std::move(grassTags)
    };

    std::vector<std::unique_ptr<TileTag>> stoneTags;
    stoneTags.emplace_back(std::make_unique<MineableTag>(3.f));
    tManager.tileTemplates["stone"] = {
        TileType::STONE,
        true,
        {0.f, 0.f},
        {.5f, .5f},
        "stone",
        {atlas->itemTexCoords["stone"]},
        nullptr,
        nullptr,
        1.f,
        std::move(stoneTags)
    };

    std::vector<std::unique_ptr<TileTag>> lavaTags = {};
    tManager.tileTemplates["lava"] = {
        TileType::LAVA,
        false,
        {0.f, 0.f},
        {1.f, 1.f},
        "none",
        {atlas->itemTexCoords["lava"]},
        game->getAssetManager()->getGlobalAnimation("lava"),
        nullptr,
        1.f,
        std::move(lavaTags)
    };

    std::vector<std::unique_ptr<TileTag>> cobbleTags;
    cobbleTags.emplace_back(std::make_unique<MineableTag>(1.f));
    tManager.tileTemplates["cobble"] = {
        TileType::COBBLE,
        false,
        {0.f, 0.f},
        {1.f, 1.f},
        "none",
        {atlas->itemTexCoords["cobble"]},
        nullptr,
        nullptr,
        1.f,
        std::move(cobbleTags)
    };

    std::vector<std::unique_ptr<TileTag>> pinkTags = {};
    tManager.tileTemplates["pink"] = {
        TileType::PINK,
        false,
        {0.f, 0.f},
        {1.f, 1.f},
        "none",
        {atlas->itemTexCoords["pink"]},
        nullptr,
        nullptr,
        1.f,
        std::move(pinkTags)
    };

    loadNearbyChunks();
}

void ChunkLayer::loadNearbyChunks()
{
    int renderDist = game->getSettings()->chunk_render_distance;
    int loadDist = game->getSettings()->chunk_load_distance;

    sf::Vector2i currChunkPos = getCurrChunkPos();

    if (currChunkPos != lastChunkPos)
    {
        for (int y = -loadDist; y <= loadDist; y++)
        {
            for (int x = -loadDist; x <= loadDist; x++)
            {
                loadChunk({currChunkPos.x + x, currChunkPos.y + y});

                if (y >= -renderDist && y <= renderDist && x >= -renderDist && x <= renderDist)
                {
                    chunks[{currChunkPos.x + x, currChunkPos.y + y}]->state = ChunkState::ACTIVE;
                }
                else
                {
                    chunks[{currChunkPos.x + x, currChunkPos.y + y}]->state = ChunkState::ASLEEP;
                }
            }
        }
    }
}

bool ChunkLayer::loadChunk(sf::Vector2i chunkPosition)
{   
    if (chunks.find(chunkPosition) == chunks.end())
    {
        chunkGenerator.generate(chunkPosition);

        return true;
    }

    return false;
}

bool ChunkLayer::unloadChunk(sf::Vector2i chunkPosition)
{
    if (chunks.find(chunkPosition) != chunks.end())
    {
        chunks.erase(chunkPosition);

        game->getScene()->getEntityLayer()->removeAllEntitiesInChunk(chunkPosition.x, chunkPosition.y);

        return true;
    }

    return false;
}

Chunk* ChunkLayer::getChunk(sf::Vector2i chunkPosition)
{
    if (chunks.find(chunkPosition) != chunks.end())
    {
        return chunks[chunkPosition].get();
    }

    return nullptr;
}

std::array<Chunk*, 9> ChunkLayer::getNearbyChunks(sf::Vector2f position)
{
    sf::Vector2i chunkPosition = worldToChunkPosition(game, position);

    std::array<Chunk*, 9> chunks;

    for (int i = 0; i < 9; i++)
    {
        chunks[i] = getChunk({chunkPosition.x - 1 + (i % 3), chunkPosition.y - 1 + (toInt(std::floor(i / 3)))});
    }

    return chunks;
}

Tile* ChunkLayer::getTileAtPosition(sf::Vector2f position, bool activeChunksOnly)
{
    sf::Vector2i tileChunkPos(worldToChunkPosition(game, position));

    Chunk* chunk = getChunk(tileChunkPos);
    if (chunk)
    {
        if (activeChunksOnly && chunk->state != ChunkState::ACTIVE) return nullptr;

        sf::Vector2i tileLocalPos(worldToTilePosition(game, position) - sf::Vector2i(tileChunkPos.x * chunkSize, tileChunkPos.y * chunkSize));
        
        return chunk->getTile(tileLocalPos);
    }

    return nullptr;
}

void ChunkLayer::tick()
{
    for (auto& i : chunks)
    {
        if (i.second->state == ChunkState::ACTIVE)
        {
            i.second->tick();
        }
    }
}

void ChunkLayer::loadUpdate()
{
    int loadDist = game->getSettings()->chunk_load_distance;

    sf::Vector2i currChunkPos = getCurrChunkPos();

    loadNearbyChunks();

    std::vector<sf::Vector2i> chunksToDelete;
    for (auto& i : chunks)
    {
        if (abs(currChunkPos.x - i.second->getChunkPosition().x) > loadDist || abs(currChunkPos.y - i.second->getChunkPosition().y) > loadDist)
        {
            chunksToDelete.push_back(i.second->getChunkPosition());
        }
    }

    if (chunksToDelete.size() > 0)
    {
        for (int i = 0; i < chunksToDelete.size(); i++)
        {
            unloadChunk(chunksToDelete[i]);
        }
    }

    if (currChunkPos != lastChunkPos)
    {
        lastChunkPos = currChunkPos;
    }
}

void ChunkLayer::update(float dt)
{
    for (auto& i : chunks)
    {
        if (i.second->state == ChunkState::ACTIVE)
        {
            i.second->update(dt);
        }
    }
}

void ChunkLayer::draw(bool debug, int debugLayerView)
{
    std::vector<BackgroundObject*> visibleBgObjects;

    for (auto& i : chunks)
    {
        if (i.second->state == ChunkState::ACTIVE)
        {
            sf::Vector2f chunkTl = chunkToWorldPosition(game, i.second->getChunkPosition());

            if (isOnScreen(game, chunkTl, {chunkLength, chunkLength}))
            {
                i.second->draw(debug, debugLayerView);

                for (int j = 0; j < i.second->bgObjects.size(); j++)
                {
                    BackgroundObject* bgObject = &i.second->bgObjects[j];

                    if (isOnScreen(game, bgObject->rect.position, bgObject->rect.size))
                    {
                        visibleBgObjects.push_back(bgObject);
                    }
                }
            }
        }

        if (debug)
        {
            sf::Vector2f chunkPos = chunkToWorldPosition(game, i.second->getChunkPosition());

            sf::Vertex tl;
            sf::Vertex tr;
            sf::Vertex bl;
            sf::Vertex br;
    
            tl.position = chunkPos;
            tr.position = {chunkPos.x + chunkLength, chunkPos.y};
            bl.position = {chunkPos.x, chunkPos.y + chunkLength};
            br.position = {chunkPos.x + chunkLength, chunkPos.y + chunkLength};

            std::vector<sf::Vertex> chunkOutline = {tl, tr, br, bl, tl};

            window->getWindow().draw(chunkOutline.data(), chunkOutline.size(), sf::PrimitiveType::LineStrip);
        }
    }

    // sorting, creating vertices for, and drawing all visible background objects
    if (!visibleBgObjects.empty())
    {
        std::sort(visibleBgObjects.begin(), visibleBgObjects.end(), [](BackgroundObject* a, BackgroundObject* b)
        {
            return a->bottom < b->bottom;
        });
    
        bgObjectsVertices.clear();
        bgObjectsVertices.reserve(visibleBgObjects.size() * 6);

        for (int i = 0; i < visibleBgObjects.size(); i++)
        {
            BackgroundObject* curr = visibleBgObjects[i];

            std::array<sf::Vertex, 6> currObjVertices = VertexGroup::createTriangleVerts(curr->rect.position, curr->rect.size, curr->texCoords);
            bgObjectsVertices.insert(bgObjectsVertices.end(), currObjVertices.begin(), currObjVertices.end());
        }
        
        window->getWindow().draw(&bgObjectsVertices[0], bgObjectsVertices.size(), sf::PrimitiveType::Triangles, bgObjectStates);
    }

    if (debug)
    {
        sf::Vector2f cursorWorldPos = game->getInput()->cursor->getGameCursorCoords();
        sf::Vector2i cursorChunkPos = worldToChunkPosition(game, cursorWorldPos);
        Chunk* cursorChunk = getChunk(cursorChunkPos);

        if (cursorChunk && cursorChunk->state == ChunkState::ACTIVE)
        {
            for (auto b : cursorChunk->bgObjects)
            {
                if (mouseRectCollide(game, b.rect.position, b.rect.size, true))
                {
                    std::array<sf::Vertex, 8> debugBgObjectVerts = VertexGroup::createLineVerts(b.rect.position, b.rect.size, sf::Color::Red);
                    window->getWindow().draw(debugBgObjectVerts.data(), debugBgObjectVerts.size(), sf::PrimitiveType::Lines);
                }
            }
        }
    }
}

sf::Vector2i ChunkLayer::getCurrChunkPos()
{
    sf::Vector2f currWorldPos;

    auto e = game->getScene()->getEntityLayer()->player;
    if (e)
    {
        currWorldPos = e->getComponent<PositionComponent>()->position.getPosition();
    }
    else
    {
        std::vector<Entity*> entitiesWithPositions = game->getScene()->getEntityLayer()->getEntitiesWithComponent<PositionComponent>();
        e = nullptr;
        if (entitiesWithPositions.size() > 0) e = entitiesWithPositions[0];

        if (e)
        {
            currWorldPos = e->getComponent<PositionComponent>()->position.getPosition();
        }
        else
        {
            currWorldPos = {0, 0};
        }
    }

    return worldToChunkPosition(game, currWorldPos);
}