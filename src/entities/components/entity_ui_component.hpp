#pragma once

#include "entity_component.hpp"

struct EntityUIComponent : EntityComponent
{
public:
    EntityUIComponent(Entity* myEntity, std::vector<std::string> componentTypesToShow);

    void createUIFor(EntityComponent* component);

    std::map<EntityComponent*, unsigned int> m_uiManagementIDs;
};