#include "entity_chunk_system.hpp"
#include "../../core/scene.hpp"
#include <algorithm>

EntityChunkSystem::EntityChunkSystem() {}

EntityChunkSystem::EntityChunkSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void EntityChunkSystem::entityChunkInit(Entity* entity)
{
    sf::Vector2i eChunkPosition = worldToChunkPosition(game, entity->position.getPosition());
    entity->chunkPosition = eChunkPosition;

    auto eChunk = scene->getChunkLayer()->getChunk(eChunkPosition);
    if (eChunk)
    {
        eChunk->entitiesInChunk.emplace_back(entity);
    }
    else
    {
        unloadEntity(entity->ID);
    }
}

void EntityChunkSystem::tick()
{
    auto entities = entityLayer->getAllEntities();

    std::vector<int> entitiesToUnload;

    for (auto& e : *entities)
    {
        Entity* curr = e.second.get();

        tickEntityChunkSync(entitiesToUnload, curr);

        tickEntityUnload(entitiesToUnload, curr);
    }

    for (auto ID : entitiesToUnload)
    {
        unloadEntity(ID);
    }
}

void EntityChunkSystem::tickEntityChunkSync(std::vector<int>& entitiesToUnload, Entity* entity)
{
    sf::Vector2i eChunkPosition = worldToChunkPosition(game, entity->position.getPosition());
    
    // if the entity hasn't moved chunks, continue.
    if (eChunkPosition == entity->chunkPosition)
    {
        return;
    }

    // entity has moved chunks.

    auto oldEChunk = scene->getChunkLayer()->getChunk(entity->chunkPosition);
    auto eInCvec = &oldEChunk->entitiesInChunk;

    // finding entity pointer in old chunk to remove it
    auto i = std::find(eInCvec->begin(), eInCvec->end(), entity);
    if (i != eInCvec->end()) eInCvec->erase(i);

    auto newEChunk = scene->getChunkLayer()->getChunk(eChunkPosition);
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

    entity->chunkPosition = eChunkPosition;
}

void EntityChunkSystem::tickEntityUnload(std::vector<int>& entitiesToUnload, Entity* entity)
{
    auto eChunk = scene->getChunkLayer()->getChunk(entity->chunkPosition);

    if (!eChunk)
    {
        entitiesToUnload.emplace_back(entity->ID);
    }
}

void EntityChunkSystem::unloadEntity(int ID)
{
    entityLayer->removeEntity(ID);
}