#pragma once

#include "entity_component.hpp"
#include "../actions/action.hpp"

struct ActionComponent : EntityComponent
{
    ActionComponent(Entity* myEntity, std::unique_ptr<Action> mainAction, std::unique_ptr<Action> secondaryAction, float range);

    void startAction(std::string actionInput);

    float range;

    std::unique_ptr<Action> mainAction;
    std::unique_ptr<Action> secondaryAction;
};