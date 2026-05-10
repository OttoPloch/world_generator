#pragma once

#include "entity_component.hpp"
#include "../actions/action.hpp"

struct ActionComponent : EntityComponent
{
    ActionComponent(Entity* myEntity, std::unique_ptr<Action> mainAction, std::unique_ptr<Action> secondaryAction, float range);

    void update(float dt) override;

    void startAction(std::string actionInput);

    void setAction(std::string actionInput, Action action);

    float range;
private:
    std::unique_ptr<Action> mainAction;
    std::unique_ptr<Action> secondaryAction;
};