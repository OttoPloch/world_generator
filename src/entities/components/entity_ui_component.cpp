#include "entity_ui_component.hpp"
#include "../entity.hpp"
#include "../../core/game.hpp"
#include "entity_component.hpp"
#include "components.hpp"

EntityUIComponent::EntityUIComponent(Entity* myEntity, std::vector<std::string> componentTypesToShow) : EntityComponent(myEntity, "entity_ui")
{
    for (auto componentType : componentTypesToShow)
    {
        std::vector<EntityComponent*> componentsOfType = myEntity->getComponentsOfType(componentType);

        for (auto component : componentsOfType)
        {
            createUIFor(component);
        }
    }
}

void EntityUIComponent::createUIFor(EntityComponent* component)
{
    if (!component) return;

    if (auto inventoryComponent = dynamic_cast<InventoryComponent*>(component))
    {
        // auto newInventoryElement = myEntity->game->getScene()->getUILayer()->createElement(std::make_unique<UIElement>(myEntity->game, "__Entity " + std::to_string(myEntity->ID) + " inventory ui", UIPosition({10, -500}, UIOrigin::TOP_LEFT, UIAnchor::BOTTOM_LEFT)));

        // componentUI["inventory"] = std::pair<EntityComponent*, std::vector<UIElement*>>(component, {newInventoryElement});
        
        // for (int i = 0; i < inventoryComponent->inventorySize; i++)
        // {
        //     auto currSlot = inventoryComponent->getItemSlot(i);
            
        //     std::string slotText;
        //     if (currSlot.second > 0)
        //     {
        //         slotText = currSlot.first + ": " + std::to_string(currSlot.second);
        //     }
        //     else
        //     {
        //         slotText = "empty";
        //     }
            
        //     newInventoryElement->addComponent<TextComponent>(myEntity->game, newInventoryElement, UIPosition({0, 0}, UIOrigin::TOP_LEFT, UIAnchor::BOTTOM_LEFT), "//item " + std::to_string(i) + " text", 1 + i, slotText, myEntity->game->getAssetManager()->getFont("sfml_font"), 16);
        // }
    }
}