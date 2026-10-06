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
        bool l_successful;
        int l_uiManagementID = myEntity->game->getScene()->getUILayer()->getUIManagementSystem()->addObject(l_successful, "entity_inventory");

        if (l_successful)
        {
            m_uiManagementIDs[component] = l_uiManagementID;
        }
    }
}