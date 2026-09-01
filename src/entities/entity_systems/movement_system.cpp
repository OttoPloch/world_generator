#include "movement_system.hpp"
#include "../../core/game.hpp"
#include "../../core/scene.hpp"
#include "../components/components.hpp"

MovementSystem::MovementSystem() {}

MovementSystem::MovementSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void MovementSystem::tick()
{
    std::vector<int> noLongerValidEntities;

    for (auto entity : validEntities)
    {
        auto entityMovementComponent = entity->getComponent<MovementComponent>();
        
        if (!entityMovementComponent)
        {
            noLongerValidEntities.emplace_back(entity->ID);
            continue;
        }
        
        entity->position.changePosition(entityMovementComponent->velocity);
        
        sf::Vector2f movementVector = {0, 0};
        float speedMult = 1.f;
    
        if (entity->getComponent<ControlComponent>())
        {
            movementVector = game->getInput()->getMovement();
    
            if (game->getInput()->isControlPressed("SPRINT")) speedMult = entityMovementComponent->stats.sprintMultilpier;
        }
    
        if (movementVector != sf::Vector2f(0, 0))
        {
            entityMovementComponent->velocity.x = entityMovementComponent->stats.speed * speedMult * movementVector.x;
            entityMovementComponent->velocity.y = entityMovementComponent->stats.speed * speedMult * movementVector.y;
        }
        else
        {
            (std::abs(entityMovementComponent->velocity.x) > 0.001f) ? entityMovementComponent->velocity.x *= game->getSettings()->motion_friction : entityMovementComponent->velocity.x = 0.f;
            (std::abs(entityMovementComponent->velocity.y) > 0.001f) ? entityMovementComponent->velocity.y *= game->getSettings()->motion_friction : entityMovementComponent->velocity.y = 0.f;
        }
    }

    removeAllEntityIDsInVec(validEntities, noLongerValidEntities);
}

void MovementSystem::refactorEntityCache()
{
    validEntities = entityLayer->getEntitiesWithComponent<MovementComponent>();
}