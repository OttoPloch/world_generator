#include "action.hpp"
#include "../../core/game.hpp"
#include "../entity.hpp"

Action::Action(Game* game, std::string name, float rangeMultiplier, float timeToComplete, float cooldown, bool positionTrackedFromStart, bool mustHoldDown) : game(game), name(name), rangeMultiplier(rangeMultiplier), timeToComplete(timeToComplete), timeProgress(0.f), cooldownProgress(0.f), active(false), positionTrackedFromStart(positionTrackedFromStart), mustHoldDown(mustHoldDown)
{
    if (cooldown == -1.f)
    {
        this->cooldown = timeToComplete;
    }
    else
    {
        this->cooldown = cooldown;
    }
}

bool Action::start()
{
    cooldownProgress = 0;
    active = true;

    return true;
}

bool Action::update(float dt)
{
    timeProgress += dt;

    return true;
}

void Action::completeAction(Entity* actor, sf::Vector2f position)
{
    sf::Vector2f p = position;

    if (positionTrackedFromStart) p = startPosition;

    std::cout << name << " has completed at " << p.x << ", " << p.y << ".\n";
    
    reset(true);
}

void Action::reset(bool restartCooldownProgress)
{
    timeProgress = 0.f;
    if (restartCooldownProgress) cooldownProgress = 0.f;
    else cooldownProgress = cooldown;
    active = false;
}