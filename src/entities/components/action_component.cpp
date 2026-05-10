#include "action_component.hpp"
#include "entity_component.hpp"
#include "../../core/game.hpp"
#include <SFML/Window/Mouse.hpp>

ActionComponent::ActionComponent(Entity* myEntity, std::unique_ptr<Action> mainAction, std::unique_ptr<Action> secondaryAction, float range) : EntityComponent(myEntity), mainAction(std::move(mainAction)), secondaryAction(std::move(secondaryAction)), range(range) {}

void ActionComponent::update(float dt)
{
    if (mainAction->active)
    {
        mainAction->timeProgress += dt;

        if (mainAction->timeProgress >= mainAction->timeToComplete)
        {
            mainAction->completeAction(myEntity->game->getInput()->getMouseWorldPos());

            mainAction->timeProgress = 0.f;
            mainAction->active = false;
        }
    }

    if (secondaryAction->active)
    {
        secondaryAction->timeProgress += dt;

        if (secondaryAction->timeProgress >= secondaryAction->timeToComplete)
        {
            secondaryAction->completeAction(myEntity->game->getInput()->getMouseWorldPos());

            secondaryAction->timeProgress = 0.f;
            secondaryAction->active = false;
        }
    }
}

void ActionComponent::startAction(std::string actionInput)
{
    if (actionInput == "MAIN ACTION" && mainAction->timeProgress == 0.f)
    {
        mainAction->clickPosition = myEntity->game->getInput()->getMouseWorldPos();
        
        if (myEntity->game->getScene()->processActionRequest(myEntity, mainAction.get()))
        {
            mainAction->active = true;    
        }
    }
    else if (actionInput == "SECONDARY ACTION" && secondaryAction->timeProgress == 0.f)
    {
        secondaryAction->clickPosition = myEntity->game->getInput()->getMouseWorldPos();
        
        if (myEntity->game->getScene()->processActionRequest(myEntity, secondaryAction.get()))
        {
            secondaryAction->active = true;
        }
    }
}

void ActionComponent::setAction(std::string actionInput, Action action)
{
    if (actionInput == "MAIN ACTION")
    {
        mainAction = std::make_unique<Action>(action);
    }
    else if (actionInput == "SECONDARY ACTION")
    {
        secondaryAction = std::make_unique<Action>(action);
    }
}