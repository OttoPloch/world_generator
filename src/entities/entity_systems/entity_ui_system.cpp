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
        auto& componentUIElements = entity->getComponent<EntityUIComponent>()->componentUIElements;

        if (componentUIElements["inventory"].second.size() > 0)
        {
            InventoryComponent* inventoryComponent = dynamic_cast<InventoryComponent*>(componentUIElements["inventory"].first);
            
            componentUIElements["inventory"].second[0]->getComponent<TextComponent>()->setText("Stone In Inventory: " + std::to_string(inventoryComponent->getItemAmount("item.stone")));
        }
    }

    removeAllEntityIDsInVec(validEntities, noLongerValidEntities);
}

void EntityUISystem::refactorEntityCache()
{
    validEntities = entityLayer->getEntitiesWithComponent<EntityUIComponent>();
}