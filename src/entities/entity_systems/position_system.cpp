#include "position_system.hpp"
#include "../../core/scene.hpp"

PositionSystem::PositionSystem() {}

PositionSystem::PositionSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void PositionSystem::tick()
{
    auto allEntities = entityLayer->getAllEntities();

    for (auto& e : *allEntities)
    {
        e.second->lastPosition = e.second->position.getPosition();
    }
}