#include "position_system.hpp"
#include "../../core/scene.hpp"
#include "../components/position_component.hpp"

PositionSystem::PositionSystem() {}

PositionSystem::PositionSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void PositionSystem::tick()
{
    std::vector<Entity*> validEntities = entityLayer->getEntitiesWithComponent<PositionComponent>();

    for (auto e : validEntities)
    {
        auto p = e->getComponent<PositionComponent>();

        p->lastPosition = p->position.getPosition();
    }
}