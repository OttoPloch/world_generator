#include "action.hpp"

Action::Action(std::string name, float rangeMultiplier, float timeToComplete, bool positionTrackedFromClick) : name(name), rangeMultiplier(rangeMultiplier), timeToComplete(timeToComplete), timeProgress(0.f), active(false), positionTrackedFromClick(positionTrackedFromClick) {}

void Action::completeAction(sf::Vector2f position)
{
    sf::Vector2f p = position;

    if (positionTrackedFromClick) p = clickPosition;

    std::cout << name << " has completed at " << p.x << ", " << p.y << ".\n";
}