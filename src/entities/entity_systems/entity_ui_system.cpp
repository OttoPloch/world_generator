#include "entity_ui_system.hpp"
#include "../../core/scene.hpp"
#include "../entity_layer.hpp"
#include "../components/entity_ui_component.hpp"
#include "../components/inventory_component.hpp"
#include "../../ui/ui_element.hpp"

EntityUISystem::EntityUISystem() {}

EntityUISystem::EntityUISystem(Game* p_game, Scene* p_scene, UIManagementSystem* p_uiManagementSystem) : game(p_game), scene(p_scene), uiManagementSystem(p_uiManagementSystem), entityLayer(scene->getEntityLayer()) {}

void EntityUISystem::tick()
{
    std::vector<unsigned int> noLongerValidEntities;

    for (auto entity : validEntities)
    {
        auto entityUIComponent = entity->getComponent<EntityUIComponent>();
        
        if (!entityUIComponent)
        {
            noLongerValidEntities.emplace_back(entity->ID);
            continue;
        }

        auto& IDmap = entityUIComponent->m_uiManagementIDs;

        for (auto i_itr = IDmap.begin(); i_itr != IDmap.end(); i_itr++)
        {
            auto l_component = i_itr->first;
            auto l_ID = i_itr->second;

            if (!l_component)
            {
                uiManagementSystem->removeObject(l_ID);
                entityUIComponent->m_uiManagementIDs.erase(i_itr);
            }

            if (auto l_inventory_Component = dynamic_cast<InventoryComponent*>(l_component))
            {
                std::unordered_map<std::string, std::string> l_variables;
                l_variables["INVENTORY_SIZE"] = std::to_string(l_inventory_Component->inventorySize);

                uiManagementSystem->updateObject(l_ID, l_variables);
            }
        }
    }

    removeAllEntityIDsInVec(validEntities, noLongerValidEntities);
}

void EntityUISystem::refactorEntityCache()
{
    validEntities = entityLayer->getEntitiesWithComponent<EntityUIComponent>();
}