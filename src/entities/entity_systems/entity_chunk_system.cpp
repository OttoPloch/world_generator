#include "entity_chunk_system.hpp"
#include "../../core/scene.hpp"
#include <algorithm>

EntityChunkSystem::EntityChunkSystem() {}

EntityChunkSystem::EntityChunkSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()), allEntities(entityLayer->getAllEntities()) {}

void EntityChunkSystem::entityChunkInit(Entity* entity)
{
    sf::Vector2i entityChunkPosition = worldToChunkPosition(game, entity->position.getPosition());
    entity->chunkPosition = entityChunkPosition;

    auto entityChunk = scene->getChunkLayer()->getChunk(entityChunkPosition);
    if (entityChunk)
    {
        entityChunk->entitiesInChunk.emplace_back(entity);
    }
    else
    {
        unloadEntities({entity->ID});
    }
}

void EntityChunkSystem::tick()
{
    std::vector<int> entitiesToUnload;

    for (auto& i : *allEntities)
    {
        auto entity = i.second.get();

        tickEntityChunkSync(entitiesToUnload, entity);

        tickEntityUnload(entitiesToUnload, entity);
    }

    unloadEntities(entitiesToUnload);
}

void EntityChunkSystem::tickEntityChunkSync(std::vector<int>& entitiesToUnload, Entity* entity)
{
    sf::Vector2i entityChunkPosition = worldToChunkPosition(game, entity->position.getPosition());
    
    // if the entity hasn't moved chunks, continue.
    if (entityChunkPosition == entity->chunkPosition)
    {
        return;
    }

    // entity has moved chunks.

    auto oldEntityChunk = scene->getChunkLayer()->getChunk(entity->chunkPosition);
    auto entitiesInChunkVec = &oldEntityChunk->entitiesInChunk;

    // finding entity pointer in old chunk to remove it
    auto i = std::find(entitiesInChunkVec->begin(), entitiesInChunkVec->end(), entity);
    if (i != entitiesInChunkVec->end()) entitiesInChunkVec->erase(i);

    auto newEChunk = scene->getChunkLayer()->getChunk(entityChunkPosition);
    if (newEChunk)
    {
        // found entity's new chunk, adding entity pointer to entitiesInChunk.
        newEChunk->entitiesInChunk.emplace_back(entity);
    }
    else
    {
        // TODO: save chunks and their entities to disk.

        // can't find entity's new chunk, unload the entity.
        entitiesToUnload.emplace_back(entity->ID);
    }

    entity->chunkPosition = entityChunkPosition;
}

void EntityChunkSystem::tickEntityUnload(std::vector<int>& entitiesToUnload, Entity* entity)
{
    auto entityChunk = scene->getChunkLayer()->getChunk(entity->chunkPosition);

    if (!entityChunk)
    {
        entitiesToUnload.emplace_back(entity->ID);
    }
}

void EntityChunkSystem::unloadEntities(std::vector<int> entitiesToUnload)
{
    entityLayer->removeEntityBatch(entitiesToUnload);
}