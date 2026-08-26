#pragma once

// TEMP FOR DEBUG
#include <SFML/Graphics.hpp>

class Game;
class Scene;
class EntityLayer;
class Entity;
class InventoryComponent;
class ItemComponent;

class ItemSystem
{
public:
    ItemSystem();

    ItemSystem(Game* game, Scene* scene);

    void tick();
private:
    void tickItems();

    void tickInventories();

    void tickItemCollect(std::vector<int>& itemsToRemove, Entity* itemEntity, Entity* inventoryEntity);

    void moveItemTowardsInventory(Entity* itemEntity, sf::Vector2f itemEntityPos, sf::Vector2f inventoryEntityPos, float distance);

    void pickupItem(std::vector<int>& itemsToRemove, InventoryComponent* inventory, ItemComponent* item, Entity* itemEntity);

    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;
};