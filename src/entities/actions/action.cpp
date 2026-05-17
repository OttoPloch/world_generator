#include "action.hpp"
#include "../../core/game.hpp"
#include "../entity.hpp"

Action::Action(std::string name, float rangeMultiplier, float timeToComplete, float cooldown, bool positionTrackedFromStart, bool mustHoldDown) : name(name), rangeMultiplier(rangeMultiplier), timeToComplete(timeToComplete), timeProgress(0.f), cooldownProgress(0.f), active(false), positionTrackedFromStart(positionTrackedFromStart), mustHoldDown(mustHoldDown)
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

bool Action::start(Game* game)
{
    return true;
}

bool Action::update(float dt, Game* game)
{
    timeProgress += dt;

    return true;
}

void Action::completeAction(Entity* actor, sf::Vector2f position)
{
    sf::Vector2f p = position;

    if (positionTrackedFromStart) p = startPosition;

    std::cout << name << " has completed at " << p.x << ", " << p.y << ".\n";
}