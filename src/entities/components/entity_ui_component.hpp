#pragma once

#include "entity_component.hpp"

struct EntityUIComponent : EntityComponent
{
public:
    EntityUIComponent(Entity* myEntity, std::vector<std::string> componentTypesToShow);

    void createUIFor(EntityComponent* component);

    std::map<std::string, std::pair<EntityComponent*, std::vector<UIElement*>>> componentUIElements;
};