#include "entity_unload_system.hpp"
#include "../../core/scene.hpp"
#include "../components/position_component.hpp"

EntityUnloadSystem::EntityUnloadSystem() {}

EntityUnloadSystem::EntityUnloadSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void EntityUnloadSystem::tick()
{
    std::vector<Entity*> validEntities = entityLayer->getEntitiesWithComponent<PositionComponent>();

    for (auto e : validEntities)
    {
        auto p = e->getComponent<PositionComponent>();

        Chunk* eChunk = scene->getChunkLayer()->getChunk(worldToChunkPosition(game, p->position.getPosition()));

        if (!eChunk)
        {
            entityLayer->removeEntity(e->ID);
        }
    }
}