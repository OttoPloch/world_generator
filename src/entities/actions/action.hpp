#pragma once

#include "../../core/common.hpp"

struct Action
{
    Action(std::string name, float rangeMultiplier, float timeToComplete, float cooldown = -1.f, bool positionTrackedFromStart = false, bool mustHoldDown = false);

    virtual void start(Game* game);

    // true if still updating, false if action was CANCELLED.
    virtual bool update(float dt, Game* game);

    virtual void completeAction(Entity* actor, sf::Vector2f position = {0, 0});

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

    bool positionTrackedFromStart;
    bool mustHoldDown;
    sf::Vector2f startPosition;
};