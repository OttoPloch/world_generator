#include "position_system.hpp"
#include "../../core/scene.hpp"

PositionSystem::PositionSystem() {}

PositionSystem::PositionSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()), allEntities(entityLayer->getAllEntities()) {}

void PositionSystem::tick()
{
    for (auto& i : *allEntities)
    {
        auto entity = i.second.get();

        entity->lastPosition = entity->position.getPosition();
    }
}