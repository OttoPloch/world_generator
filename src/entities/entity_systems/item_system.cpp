#include "item_system.hpp"

#include "../../core/scene.hpp"
#include "../components/item_component.hpp"
#include "../components/collision_component.hpp"

ItemSystem::ItemSystem() {}

ItemSystem::ItemSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void ItemSystem::tick()
{
    std::vector<Entity*> validEntities = entityLayer->getEntitiesWithComponents<ItemComponent, MovementComponent>();

    for (auto e : validEntities)
    {
        auto m = e->getComponent<MovementComponent>();

        if (m->velocity == sf::Vector2f(0, 0))
        {
            e->removeComponent<MovementComponent>();

            // Also remove collision component if the item has one, since the item is not going
            // to move again and doesn't really need to check for collision. Maybe temp, but a
            // good optimization for now I think.
            if (e->getComponent<CollisionComponent>())
            {
                e->removeComponent<CollisionComponent>();
            }
        }
    }
}