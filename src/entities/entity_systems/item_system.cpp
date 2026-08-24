#include "item_system.hpp"

#include "../../core/scene.hpp"
#include "../components/item_component.hpp"
#include "../components/collision_component.hpp"
#include "../components/inventory_component.hpp"
#include "../components/position_component.hpp"
#include "../components/sprite_component.hpp"
#include "../../core/game.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <cmath>

ItemSystem::ItemSystem() {}

ItemSystem::ItemSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void ItemSystem::tick()
{
    tickItems();

    tickInventories();
}

void ItemSystem::tickItems()
{
    std::vector<Entity*> validEntities = entityLayer->getEntitiesWithComponents<ItemComponent, MovementComponent>();

    for (auto e : validEntities)
    {
        auto m = e->getComponent<MovementComponent>();

        if (m->velocity == sf::Vector2f(0, 0))
        {
            e->removeComponent<MovementComponent>();
        }
    }
}

void ItemSystem::tickInventories()
{
    std::vector<Entity*> validEntities = entityLayer->getEntitiesWithComponents<InventoryComponent, PositionComponent>();
    std::vector<Entity*> items = entityLayer->getEntitiesWithComponents<ItemComponent, PositionComponent>();

    for (auto e : validEntities)
    {
        auto p = e->getComponent<PositionComponent>();
        auto i = e->getComponent<InventoryComponent>();
        sf::Vector2f ePos = p->position.getPosition();

        for (auto item : items)
        {
            auto itemI = item->getComponent<ItemComponent>();
            if (i->canPickup(itemI->resource) == 0) continue;

            // std::cout << itemI << '\n';

            auto itemP = item->getComponent<PositionComponent>();
            sf::Vector2f itemPos = itemP->position.getPosition();

            float distance = getDistance(ePos, itemPos);
            if (distance > i->pickupRange)
            {
                continue;
            }
            else if (distance > 2)
            {
                auto m = item->getComponent<MovementComponent>();

                if (!m)
                {
                    m = item->addComponent<MovementComponent>(item, sf::Vector2f(0, 0), MovementComponentData({0, 0}));
                }

                float angle = getAngle(itemPos, ePos);

                float velocityMultiplier = game->getSettings()->item_collectMoveMultiplier;
                sf::Vector2f itemVelocity(std::cos(angle) * velocityMultiplier / distance, -std::sin(angle) * velocityMultiplier / distance);

                m->velocity += itemVelocity;
            }
            else
            {
                unsigned int extra;
                i->pickupItem(itemI->resource, itemI->amount, extra);

                itemI->amount = extra;

                if (itemI->amount == 0) entityLayer->removeEntity(item->ID);
            }
        }
    }
}