#include "entity_ui_system.hpp"
#include "../../core/scene.hpp"
#include "../entity_layer.hpp"
#include "../components/entity_ui_component.hpp"
#include "../components/inventory_component.hpp"
#include "../../ui/ui_element.hpp"

EntityUISystem::EntityUISystem() {}

EntityUISystem::EntityUISystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void EntityUISystem::tick()
{
    std::vector<int> noLongerValidEntities;

    for (auto entity : validEntities)
    {
        auto entityUIComponent = entity->getComponent<EntityUIComponent>();
        
        if (!entityUIComponent)
        {
            noLongerValidEntities.emplace_back(entity->ID);
            continue;
        }

        auto& componentUIMap = entityUIComponent->componentUI;

        for (auto& componentUI : componentUIMap)
        {
            if (componentUI.first == "inventory")
            {
                auto inventoryComponent = dynamic_cast<InventoryComponent*>(componentUI.second.first);

                if (!inventoryComponent) continue;

                for (int i = 0; i < inventoryComponent->inventorySize; i++)
                {
                    if (auto textComponent = componentUI.second.second[0]->getComponent<TextComponent>("//item " + std::to_string(i) + " text"))
                    {
                        auto currSlot = inventoryComponent->getItemSlot(i);

                        std::string slotText;
                        if (currSlot.second > 0)
                        {
                            slotText = currSlot.first.substr(5) + ": " + std::to_string(currSlot.second);
                        }
                        else
                        {
                            slotText = "empty";
                        }

                        textComponent->setText(slotText);
                    }
                }
            }
        }
    }

    removeAllEntityIDsInVec(validEntities, noLongerValidEntities);
}

void EntityUISystem::refactorEntityCache()
{
    validEntities = entityLayer->getEntitiesWithComponent<EntityUIComponent>();
}