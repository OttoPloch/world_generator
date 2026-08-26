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
#include <algorithm>

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
    std::vector<Entity*> itemEntities = entityLayer->getEntitiesWithComponents<ItemComponent, PositionComponent>();

    std::vector<int> itemsToRemove;

    for (auto inventoryEntity : validEntities)
    {
        for (auto itemEntity : itemEntities)
        {
            // skip items that have been picked up
            if (std::find(itemsToRemove.begin(), itemsToRemove.end(), itemEntity->ID) == itemsToRemove.end())
            {
                tickItemCollect(itemsToRemove, itemEntity, inventoryEntity);
            }
        }
    }

    for (auto itemID : itemsToRemove)
    {
        entityLayer->removeEntity(itemID);
    }
}

void ItemSystem::tickItemCollect(std::vector<int>& itemsToRemove, Entity* itemEntity, Entity* inventoryEntity)
{
    auto inventory = inventoryEntity->getComponent<InventoryComponent>();

    sf::Vector2f inventoryEntityPos = inventoryEntity->getComponent<PositionComponent>()->position.getPosition();
    sf::Vector2f itemEntityPos = itemEntity->getComponent<PositionComponent>()->position.getPosition();

    auto item = itemEntity->getComponent<ItemComponent>();
    if (inventory->canPickup(item->resource) == 0) return;

    float distance = getDistance(inventoryEntityPos, itemEntityPos);
    if (distance > inventory->pickupRange) return;
    
    bool pickUpItem = false;
    
    auto inventoryEntityCollider = inventoryEntity->getComponent<CollisionComponent>();
    auto itemEntityCollider = itemEntity->getComponent<CollisionComponent>();
    if (inventoryEntityCollider && itemEntityCollider)
    {
        sf::FloatRect inventoryEntityRect(inventoryEntityCollider->rect.position.getPosition(), inventoryEntityCollider->rect.size);
        sf::FloatRect itemEntityRect(itemEntityCollider->rect.position.getPosition(), itemEntityCollider->rect.size);

        if (rectRectCollide(inventoryEntityRect, itemEntityRect, true))
        {
            pickUpItem = true;
        }
    }
    else if (distance < game->getSettings()->tile_size / 5)
    {
        pickUpItem = true;
    }

    if (pickUpItem)
    {
        pickupItem(itemsToRemove, inventory, item, itemEntity);
    }
    else
    {
        moveItemTowardsInventory(itemEntity, itemEntityPos, inventoryEntityPos, distance);
    }
}

void ItemSystem::moveItemTowardsInventory(Entity* itemEntity, sf::Vector2f itemEntityPos, sf::Vector2f inventoryEntityPos, float distance)
{
    auto m = itemEntity->getComponent<MovementComponent>();

    if (!m)
    {
        m = itemEntity->addComponent<MovementComponent>(itemEntity, sf::Vector2f(0, 0), MovementComponentData({0, 0}));
    }

    float angle = getAngle(itemEntityPos, inventoryEntityPos);

    float velocityMultiplier = game->getSettings()->item_collectMoveMultiplier;
    sf::Vector2f itemVelocity(std::cos(angle) * velocityMultiplier / distance, -std::sin(angle) * velocityMultiplier / distance);

    m->velocity += itemVelocity;
}

void ItemSystem::pickupItem(std::vector<int>& itemsToRemove, InventoryComponent* inventory, ItemComponent* item, Entity* itemEntity)
{
    unsigned int extra;
    inventory->pickupItem(item->resource, item->amount, extra);

    item->amount = extra;

    if (item->amount == 0) itemsToRemove.emplace_back(itemEntity->ID);
}