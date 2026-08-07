#include "action_component.hpp"
#include "entity_component.hpp"
#include "../../core/game.hpp"
#include <SFML/Window/Mouse.hpp>

ActionComponent::ActionComponent(Entity* myEntity, std::unique_ptr<Action> mainAction, std::unique_ptr<Action> secondaryAction, float range) : EntityComponent(myEntity), mainAction(std::move(mainAction)), secondaryAction(std::move(secondaryAction)), range(range) {}

void ActionComponent::startAction(std::string actionInput)
{
    if (actionInput == "MAIN ACTION" && mainAction->cooldownProgress >= mainAction->cooldown && !mainAction->active)
    {
        mainAction->startPosition = myEntity->game->getInput()->cursor->getGameCursorCoords();
        
        if (myEntity->game->getScene()->processActionRequest(myEntity, mainAction.get()))
        {
            mainAction->start();
        }
    }
    else if (actionInput == "SECONDARY ACTION" && secondaryAction->cooldownProgress >= secondaryAction->cooldown && !secondaryAction->active)
    {
        secondaryAction->startPosition = myEntity->game->getInput()->cursor->getGameCursorCoords();
        
        if (myEntity->game->getScene()->processActionRequest(myEntity, secondaryAction.get()))
        {
            secondaryAction->start();
        }
    }
}