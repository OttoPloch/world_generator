#include "action_system.hpp"
#include "../../core/game.hpp"
#include "../../core/scene.hpp"
#include "../components/components.hpp"

ActionSystem::ActionSystem() {}

ActionSystem::ActionSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void ActionSystem::update(float dt)
{
    // player actions
    if (entityLayer->player)
    {
        if (auto a = entityLayer->player->getComponent<ActionComponent>())
        {
            if (game->getInput()->isControlPressed("MAIN ACTION"))
            {
                if (!scene->getUILayer()->checkUICollision())
                {
                    if (entityLayer->player)
                    {
                        a->startAction("MAIN ACTION");
                    }
                }
            }
            else if (game->getInput()->isControlPressed("SECONDARY ACTION"))
            {
                if (!scene->getUILayer()->checkUICollision())
                {
                    if (entityLayer->player)
                    {
                        a->startAction("SECONDARY ACTION");
                    }
                }
            }
        }
    }

    std::vector<Entity*> validEntities = entityLayer->getEntitiesWithComponent<ActionComponent>();

    for (auto e : validEntities)
    {
        auto a = e->getComponent<ActionComponent>();

        a->mainAction->cooldownProgress += dt;
        a->secondaryAction->cooldownProgress += dt;
    
        Action* currentAction;
        std::string currentActionInputName;
        for (int i = 0; i < 2; i++)
        {
            if (i == 0)
            {
                currentAction = a->mainAction.get();
                currentActionInputName = "MAIN ACTION";
            }
            else if (i == 1)
            {
                currentAction = a->secondaryAction.get();
                currentActionInputName = "SECONDARY ACTION";
            }
            
            if (currentAction->active)
            {
                if (currentAction->mustHoldDown && !game->getInput()->isControlPressed(currentActionInputName))
                {
                    currentAction->cooldownProgress = currentAction->cooldown;
                    currentAction->timeProgress = 0.f;
                    currentAction->active = false;
                }
                else
                {
                    if (currentAction->update(dt, game))
                    {
                        if (currentAction->timeProgress >= currentAction->timeToComplete)
                        {
                            currentAction->completeAction(e, game->getInput()->cursor->getGameCursorCoords());
                
                            currentAction->timeProgress = 0.f;
                            currentAction->active = false;
                        }
                    }
                    else
                    {
                        currentAction->active = false;
                        currentAction->timeProgress = 0.f;
                        currentAction->timeToComplete = 0.f;
                        currentAction->cooldownProgress = 0.f;
                        currentAction->cooldown = 0.f;
                    }
                }
            }
        }
    }
}