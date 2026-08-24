#pragma once

#include "../../core/common.hpp"
#include "entity_component.hpp"

struct InventoryComponent : EntityComponent
{
    InventoryComponent(Entity* myEntity, unsigned int inventorySize, float pickupRange, bool rangeIsInTiles = true);

    void pickupItem(std::string item, unsigned int amount, unsigned int& extra);

    unsigned int canPickup(std::string item);
    
    unsigned int getItemAmount(std::string item);

    unsigned int inventorySize;
    float pickupRange;
    unsigned int stackSize;
private:
    void resetInventory();

    std::vector<std::pair<std::string, unsigned int>> items;
};