#pragma once

#include "../../core/common.hpp"

struct Action
{
    Action(std::string name, float rangeMultiplier, float timeToComplete, float cooldown = -1.f, bool positionTrackedFromClick = false);

    virtual void completeAction(sf::Vector2f position = {0, 0});

    std::string name;

    // the multiplier for the range
    // of the action component of the
    // entity that does this action.
    // negative = inf range
    float rangeMultiplier;
    
    float timeToComplete;
    float timeProgress;

    float cooldown;
    float cooldownProgress;

    bool active;

    bool positionTrackedFromClick;
    sf::Vector2f clickPosition;
};