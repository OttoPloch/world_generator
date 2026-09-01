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
                a->startAction("MAIN ACTION");
            }
            else if (game->getInput()->isControlPressed("SECONDARY ACTION"))
            {
                a->startAction("SECONDARY ACTION");
            }
        }
    }

    std::vector<int> noLongerValidEntities;

    for (auto entity : validEntities)
    {
        auto entityActionComponent = entity->getComponent<ActionComponent>();

        if (!entityActionComponent)
        {
            noLongerValidEntities.emplace_back(entity->ID);
            continue;
        }

        if (entityActionComponent->mainAction && !entityActionComponent->mainAction->active) entityActionComponent->mainAction->cooldownProgress += dt;
        if (entityActionComponent->secondaryAction && !entityActionComponent->secondaryAction->active) entityActionComponent->secondaryAction->cooldownProgress += dt;
    
        Action* currentAction;
        std::string currentActionInputName;
        for (int i = 0; i < 2; i++)
        {
            if (i == 0)
            {
                if (!entityActionComponent->mainAction) continue;

                currentAction = entityActionComponent->mainAction.get();
                currentActionInputName = "MAIN ACTION";
            }
            else if (i == 1)
            {
                if (!entityActionComponent->secondaryAction) continue;

                currentAction = entityActionComponent->secondaryAction.get();
                currentActionInputName = "SECONDARY ACTION";
            }
            
            if (!currentAction->active) continue;
            
            if (currentAction->mustHoldDown && !game->getInput()->isControlPressed(currentActionInputName))
            {
                currentAction->reset(false);
            }
            else
            {
                if (currentAction->update(dt))
                {
                    if (currentAction->timeProgress >= currentAction->timeToComplete)
                    {
                        currentAction->completeAction(entity, game->getInput()->cursor->getGameCursorCoords());
                    }
                }
                else
                {
                    currentAction->reset(false);
                }
            }
        }
    }

    removeAllEntityIDsInVec(validEntities, noLongerValidEntities);
}

void ActionSystem::refactorEntityCache()
{
    validEntities = entityLayer->getEntitiesWithComponent<ActionComponent>();
}