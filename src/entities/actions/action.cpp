#include "action.hpp"
#include "../../core/game.hpp"
#include "../entity.hpp"

Action::Action(std::string name, float rangeMultiplier, float timeToComplete, float cooldown, bool positionTrackedFromClick) : name(name), rangeMultiplier(rangeMultiplier), timeToComplete(timeToComplete), timeProgress(0.f), cooldownProgress(0.f), active(false), positionTrackedFromClick(positionTrackedFromClick)
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

void Action::completeAction(Entity* actor, sf::Vector2f position)
{
    sf::Vector2f p = position;

    if (positionTrackedFromClick) p = clickPosition;

    std::cout << name << " has completed at " << p.x << ", " << p.y << ".\n";

    EntityLayer* eL = actor->game->getScene()->getEntityLayer();

    eL->addEntity(p, &eL->tManager.entityTemplates["box"]);
}