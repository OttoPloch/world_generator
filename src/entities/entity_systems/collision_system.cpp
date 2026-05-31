#include "collision_system.hpp"
#include "../../core/game.hpp"
#include "../entity_layer.hpp"
#include "../components/components.hpp"

CollisionSystem::CollisionSystem() {}

CollisionSystem::CollisionSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void CollisionSystem::update(float dt)
{
    std::vector<Entity*> validEntities = entityLayer->getEntitiesWithComponents<MovementComponent, CollisionComponent>();

    for (auto e : validEntities)
    {
        auto movement = e->getComponent<MovementComponent>();
        auto collision = e->getComponent<CollisionComponent>();
        
        std::cout << e->getPosition().x << ", " << e->getPosition().y << '\n';
    }
}