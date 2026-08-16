#pragma once

#include "../../core/common.hpp"

struct Action
{
    Action(Game* game, std::string name, float rangeMultiplier, float timeToComplete, float cooldown = -1.f, bool positionTrackedFromStart = false, bool mustHoldDown = false);

    virtual bool start();

    // true if still updating, false if action was CANCELLED.
    virtual bool update(float dt);

    virtual void completeAction(Entity* actor, sf::Vector2f position = {0, 0});

    // if restartCooldownProgress is false, then it will be set to cooldown, meaning the
    // action can be started again immediately.
    virtual void reset(bool restartCooldownProgress);

    // Must override this function in each child class to make that class cloneable.
    virtual std::unique_ptr<Action> clone();

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
protected:
    Game* game;
};