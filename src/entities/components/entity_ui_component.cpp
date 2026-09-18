#include "entity_ui_component.hpp"
#include "../entity.hpp"
#include "../../ui/ui_position.hpp"
#include "../../core/game.hpp"
#include "inventory_component.hpp"

EntityUIComponent::EntityUIComponent(Entity* myEntity, std::vector<std::string> componentTypesToShow) : EntityComponent(myEntity, "entity_ui")
{
    std::vector<EntityComponent*> componentsToCreateUIFor;

    for (std::string typeName : componentTypesToShow)
    {
        if (auto component = myEntity->getComponent(typeName))
        {
            componentsToCreateUIFor.emplace_back(component);
        }
    }

    for (auto component : componentsToCreateUIFor)
    {
        createUIFor(component);
    }
}

void EntityUIComponent::createUIFor(EntityComponent* component)
{
    if (!component) return;

    std::string componentTypeName = component->componentTypeName;

    if (componentTypeName == "inventory")
    {
        auto newInventoryElement = myEntity->game->getScene()->getUILayer()->createElement(std::make_unique<UIElement>(myEntity->game, "Entity " + std::to_string(myEntity->ID) + " " + componentTypeName + " ui", UIPosition({0, 0}, UIOrigin::TOP_LEFT, UIAnchor::BOTTOM_MIDDLE)));

        componentUIElements["inventory"] = std::pair<EntityComponent*, std::vector<UIElement*>>(component, {newInventoryElement});

        newInventoryElement->addComponent<TextComponent>(myEntity->game, newInventoryElement, UIPosition({0, 0}, UIOrigin::BOTTOM_MIDDLE), "text", 0, "Stone In Inventory: ", myEntity->game->getAssetManager()->getFont("sfml_font"), 32);
    }
}