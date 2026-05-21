#include "action_component.hpp"
#include "entity_component.hpp"
#include "../../core/game.hpp"
#include <SFML/Window/Mouse.hpp>

ActionComponent::ActionComponent(Entity* myEntity, std::unique_ptr<Action> mainAction, std::unique_ptr<Action> secondaryAction, float range) : EntityComponent(myEntity), mainAction(std::move(mainAction)), secondaryAction(std::move(secondaryAction)), range(range) {}

void ActionComponent::update(float dt)
{
    mainAction->cooldownProgress += dt;
    secondaryAction->cooldownProgress += dt;

    if (mainAction->active)
    {
        if (mainAction->mustHoldDown && !myEntity->game->getInput()->isControlPressed("MAIN ACTION"))
        {
            mainAction->cooldownProgress = mainAction->cooldown;
            mainAction->timeProgress = 0.f;
            mainAction->active = false;
        }
        else
        {
            if (mainAction->update(dt, myEntity->game))
            {
                if (mainAction->timeProgress >= mainAction->timeToComplete)
                {
                    mainAction->completeAction(myEntity, myEntity->game->getInput()->getMouseWorldPos());
        
                    mainAction->timeProgress = 0.f;
                    mainAction->active = false;
                }
            }
            else
            {
                mainAction->active = false;
                mainAction->timeProgress = 0.f;
                mainAction->timeToComplete = 0.f;
                mainAction->cooldownProgress = 0.f;
                mainAction->cooldown = 0.f;
            }
        }
    }

    if (secondaryAction->active)
    {
        if (secondaryAction->mustHoldDown && !myEntity->game->getInput()->isControlPressed("SECONDARY ACTION"))
        {
            secondaryAction->cooldownProgress = secondaryAction->cooldown;
            secondaryAction->timeProgress = 0.f;
            secondaryAction->active = false;
        }
        else
        {
            if (secondaryAction->update(dt, myEntity->game))
            {
                if (secondaryAction->timeProgress >= secondaryAction->timeToComplete)
                {
                    secondaryAction->completeAction(myEntity, myEntity->game->getInput()->getMouseWorldPos());
        
                    secondaryAction->timeProgress = 0.f;
                    secondaryAction->active = false;
                }
            }
            else
            {
                secondaryAction->active = false;
                secondaryAction->timeProgress = 0.f;
                secondaryAction->timeToComplete = 0.f;
                secondaryAction->cooldownProgress = 0.f;
                secondaryAction->cooldown = 0.f;
            }
        }
    }
}

void ActionComponent::startAction(std::string actionInput)
{
    if (actionInput == "MAIN ACTION" && mainAction->cooldownProgress >= mainAction->cooldown)
    {
        mainAction->startPosition = myEntity->game->getInput()->getMouseWorldPos();
        
        if (myEntity->game->getScene()->processActionRequest(myEntity, mainAction.get()))
        {
            mainAction->active = true;
            mainAction->cooldownProgress = 0.f;
            mainAction->start(myEntity->game);
        }
    }
    else if (actionInput == "SECONDARY ACTION" && secondaryAction->cooldownProgress >= secondaryAction->cooldown)
    {
        secondaryAction->startPosition = myEntity->game->getInput()->getMouseWorldPos();
        
        if (myEntity->game->getScene()->processActionRequest(myEntity, secondaryAction.get()))
        {
            secondaryAction->active = true;
            secondaryAction->cooldownProgress = 0.f;
            secondaryAction->start(myEntity->game);
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