#include "movement_system.hpp"
#include "../../core/game.hpp"
#include "../../core/scene.hpp"
#include "../components/components.hpp"

MovementSystem::MovementSystem() {}

MovementSystem::MovementSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void MovementSystem::tick()
{
    std::vector<unsigned int> noLongerValidEntities;

    for (auto entity : validEntities)
    {
        auto entityMovementComponent = entity->getComponent<MovementComponent>();
        
        if (!entityMovementComponent)
        {
            noLongerValidEntities.emplace_back(entity->ID);
            continue;
        }
        
        entity->position.changePosition(entityMovementComponent->velocity);
        
        float speedMult = 1.f;
    
        if (entity->getComponent<ControlComponent>())
        {
            entityMovementComponent->movementInput = game->getInputManager()->getMovement();

            if (game->getInputManager()->isControlPressed("SPRINT")) speedMult = entityMovementComponent->stats.sprintMultilpier;
        }

        if (entityMovementComponent->movementInput != sf::Vector2f(0, 0)) entityMovementComponent->movementInput = entityMovementComponent->movementInput.normalized();
    
        (std::abs(entityMovementComponent->velocity.x) > 0.001f) ? entityMovementComponent->velocity.x *= game->getSettings()->motion_friction : entityMovementComponent->velocity.x = 0.f;
        (std::abs(entityMovementComponent->velocity.y) > 0.001f) ? entityMovementComponent->velocity.y *= game->getSettings()->motion_friction : entityMovementComponent->velocity.y = 0.f;
        
        if (entityMovementComponent->movementInput != sf::Vector2f(0, 0))
        {
            entityMovementComponent->velocity.x = entityMovementComponent->stats.speed * speedMult * entityMovementComponent->movementInput.x;
            entityMovementComponent->velocity.y = entityMovementComponent->stats.speed * speedMult * entityMovementComponent->movementInput.y;
        }

        entityMovementComponent->movementInput = {0, 0};
    }

    removeAllEntityIDsInVec(validEntities, noLongerValidEntities);
}

void MovementSystem::refactorEntityCache()
{
    validEntities = entityLayer->getEntitiesWithComponent<MovementComponent>();
}