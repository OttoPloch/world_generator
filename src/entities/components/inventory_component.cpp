#include "inventory_component.hpp"
#include "../entity.hpp"
#include "../../core/game.hpp"
#include <algorithm>

InventoryComponent::InventoryComponent(Entity* myEntity, unsigned int inventorySize, float pickupRange, bool rangeIsInTiles) : EntityComponent(myEntity), inventorySize(inventorySize)
{
    this->pickupRange = pickupRange;
    if (rangeIsInTiles) this->pickupRange *= myEntity->game->getSettings()->tile_size;

    stackSize = myEntity->game->getSettings()->item_stackSize;

    resetInventory();
}

void InventoryComponent::pickupItem(std::string item, unsigned int amount, unsigned int& extra)
{
    if (canPickup(item) == 0) return;
    
    extra = amount;
    
    // First fill slots that have the item already.
    for (auto& i : items)
    {
        if (i.first != item) continue;
        if (i.second >= stackSize) continue;
        
        unsigned int diff = stackSize - i.second;
        unsigned int amountToAdd = std::min(diff, extra);
        
        i.second += amountToAdd;
        extra -= amountToAdd;

        if (extra <= 0) break;
    }

    if (extra > 0)
    {
        // Then fill empty slots.
        for (auto& i : items)
        {
            if (i.first != "none") continue;
    
            i.first = item;
    
            unsigned int amountToAdd = std::min(stackSize, extra);
            
            i.second += amountToAdd;
            extra -= amountToAdd;
    
            if (extra <= 0) break;
        }
    }

    std::string amountMessage = std::to_string(static_cast<unsigned int>(getItemAmount(item) / stackSize)) + " stacks and " + std::to_string (getItemAmount(item) % stackSize);
    // if (amount - extra > 0) std::cout << "entity with ID " << myEntity->ID << " picked up " << amount - extra << " " << item << ". This entity now has " << amountMessage << " " << item << "\n";
    
    std::cout << "---------------\nEntity with ID " << myEntity->ID << ":\n"; for (auto i : items) std::cout << i.first << ": " << i.second << '\n'; std::cout << "---------------\n";
    
    if (extra > 0) std::cout << "could not pick up all of '" << item << "'. There is " << extra << " extra.\n";
}

unsigned int InventoryComponent::canPickup(std::string item)
{
    unsigned int availableSpace = 0;

    for (auto& i : items)
    {
        if (i.first == "none")
        {
            availableSpace += stackSize;
        }
        else if (i.first == item)
        {
            if (i.second < stackSize)
            {
                availableSpace += stackSize - i.second;
            }
        }
    }

    return availableSpace;
}

unsigned int InventoryComponent::getItemAmount(std::string item)
{
    unsigned int amount = 0;

    for (auto& i : items)
    {
        if (i.first == item)
        {
            amount += i.second;
        }
    }

    return amount;
}

void InventoryComponent::resetInventory()
{
    items.clear();
    for (int i = 0; i < inventorySize; i++)
    {
        items.emplace_back("none", 0);
    }
}