#include "movement_system.hpp"
#include "../../core/game.hpp"
#include "../../core/scene.hpp"
#include "../components/components.hpp"

MovementSystem::MovementSystem() {}

MovementSystem::MovementSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void MovementSystem::tick()
{
    std::vector<Entity*> validEntities = entityLayer->getEntitiesWithComponent<MovementComponent>();

    for (auto e : validEntities)
    {
        auto m = e->getComponent<MovementComponent>();
        e->position.changePosition(m->velocity);
        
        sf::Vector2f movementVector = {0, 0};
        float speedMult = 1.f;
    
        if (e->getComponent<ControlComponent>())
        {
            movementVector = game->getInput()->getMovement();
    
            if (game->getInput()->isControlPressed("SPRINT")) speedMult = m->stats.sprintMultilpier;
        }
    
        if (movementVector != sf::Vector2f(0, 0))
        {
            m->velocity.x = m->stats.speed * speedMult * movementVector.x;
            m->velocity.y = m->stats.speed * speedMult * movementVector.y;
        }
        else
        {
            (std::abs(m->velocity.x) > 0.001f) ? m->velocity.x *= game->getSettings()->motion_friction : m->velocity.x = 0.f;
            (std::abs(m->velocity.y) > 0.001f) ? m->velocity.y *= game->getSettings()->motion_friction : m->velocity.y = 0.f;
        }
    }
}