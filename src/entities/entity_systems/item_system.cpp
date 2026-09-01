#include "item_system.hpp"

#include "../../core/scene.hpp"
#include "../components/item_component.hpp"
#include "../components/collision_component.hpp"
#include "../components/inventory_component.hpp"
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

void ItemSystem::refactorEntityCache()
{
    itemsWithMovement = entityLayer->getEntitiesWithComponents<ItemComponent, MovementComponent>();
    entitiesWithInventories = entityLayer->getEntitiesWithComponent<InventoryComponent>();
    items = entityLayer->getEntitiesWithComponent<ItemComponent>();
}

void ItemSystem::tickItems()
{
    std::vector<int> noLongerValidEntities;

    for (auto entity : itemsWithMovement)
    {
        auto entityMovementComponent = entity->getComponent<MovementComponent>();

        if (!entity->getComponent<ItemComponent>() || !entityMovementComponent)
        {
            noLongerValidEntities.emplace_back(entity->ID);
            continue;
        }

        if (entityMovementComponent->velocity == sf::Vector2f(0, 0))
        {
            entity->removeComponent<MovementComponent>();
        }
    }

    removeAllEntityIDsInVec(itemsWithMovement, noLongerValidEntities);
}

void ItemSystem::tickInventories()
{
    std::vector<int> noLongerValidInventoryEntities;
    std::vector<int> noLongerValidItemEntities;

    std::vector<int> itemsToRemove;

    for (auto inventoryEntity : entitiesWithInventories)
    {
        if (!inventoryEntity->getComponent<InventoryComponent>())
        {
            noLongerValidInventoryEntities.emplace_back(inventoryEntity->ID);
            continue;
        }

        for (auto itemEntity : items)
        {
            if (!itemEntity->getComponent<ItemComponent>())
            {
                noLongerValidItemEntities.emplace_back(itemEntity->ID);
                continue;
            }

            // skip items that have been picked up
            if (std::find(itemsToRemove.begin(), itemsToRemove.end(), itemEntity->ID) == itemsToRemove.end())
            {
                tickItemCollect(itemsToRemove, itemEntity, inventoryEntity);
            }
        }
    }

    entityLayer->removeEntityBatch(itemsToRemove);

    removeAllEntityIDsInVec(entitiesWithInventories, noLongerValidInventoryEntities);
    removeAllEntityIDsInVec(items, noLongerValidItemEntities);
}

void ItemSystem::tickItemCollect(std::vector<int>& itemsToRemove, Entity* itemEntity, Entity* inventoryEntity)
{
    auto inventoryEntityInventoryComponent = inventoryEntity->getComponent<InventoryComponent>();

    sf::Vector2f inventoryEntityPos = inventoryEntity->position.getPosition();
    sf::Vector2f itemEntityPos = itemEntity->position.getPosition();

    auto itemEntityItemComponent = itemEntity->getComponent<ItemComponent>();
    if (inventoryEntityInventoryComponent->canPickup(itemEntityItemComponent->resource) == 0) return;

    float distance = getDistance(inventoryEntityPos, itemEntityPos);
    if (distance > inventoryEntityInventoryComponent->pickupRange) return;
    
    bool pickUpItem = false;
    
    auto inventoryEntityCollisionComponent = inventoryEntity->getComponent<CollisionComponent>();
    auto itemEntityCollisionComponent = itemEntity->getComponent<CollisionComponent>();
    if (inventoryEntityCollisionComponent && itemEntityCollisionComponent)
    {
        sf::FloatRect inventoryEntityRect(inventoryEntityCollisionComponent->rect.position.getPosition(), inventoryEntityCollisionComponent->rect.size);
        sf::FloatRect itemEntityRect(itemEntityCollisionComponent->rect.position.getPosition(), itemEntityCollisionComponent->rect.size);

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
        pickupItem(itemsToRemove, inventoryEntityInventoryComponent, itemEntityItemComponent, itemEntity);
    }
    else
    {
        moveItemTowardsInventory(itemEntity, itemEntityPos, inventoryEntityPos, distance);
    }
}

void ItemSystem::moveItemTowardsInventory(Entity* itemEntity, sf::Vector2f itemEntityPos, sf::Vector2f inventoryEntityPos, float distance)
{
    auto itemEntityMovementComponent = itemEntity->getComponent<MovementComponent>();

    if (!itemEntityMovementComponent)
    {
        itemEntityMovementComponent = itemEntity->addComponent<MovementComponent>(itemEntity, sf::Vector2f(0, 0), MovementComponentData({0, 0}));
    }

    float angle = getAngle(itemEntityPos, inventoryEntityPos);

    float velocityMultiplier = game->getSettings()->item_collectMoveMultiplier;
    sf::Vector2f itemVelocity(std::cos(angle) * velocityMultiplier / distance, -std::sin(angle) * velocityMultiplier / distance);

    itemEntityMovementComponent->velocity += itemVelocity;
}

void ItemSystem::pickupItem(std::vector<int>& itemsToRemove, InventoryComponent* inventoryEntityInventoryComponent, ItemComponent* itemEntityItemComponent, Entity* itemEntity)
{
    unsigned int extra;
    inventoryEntityInventoryComponent->pickupItem(itemEntityItemComponent->resource, itemEntityItemComponent->amount, extra);

    itemEntityItemComponent->amount = extra;

    if (itemEntityItemComponent->amount == 0) itemsToRemove.emplace_back(itemEntity->ID);
}