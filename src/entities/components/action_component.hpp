#pragma once

#include "entity_component.hpp"

struct ActionComponent : EntityComponent
{
    ActionComponent(Entity* myEntity, std::string mainAction, std::string secondaryAction);

    void setAction(std::string action, bool secondary = false);

    void createRequest(std::string actionInput);

    // temp, need to make Action struct and substructs.
    std::string mainAction;
    std::string secondaryAction;
};