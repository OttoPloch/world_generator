#include "movement_system.hpp"
#include "../../core/game.hpp"
#include "../../core/scene.hpp"
#include "../components/components.hpp"

MovementSystem::MovementSystem() {}

MovementSystem::MovementSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void MovementSystem::tick()
{
    std::vector<Entity*> validEntities = entityLayer->getEntitiesWithComponents<MovementComponent, PositionComponent>();

    for (auto e : validEntities)
    {
        auto m = e->getComponent<MovementComponent>();
        auto p = e->getComponent<PositionComponent>();

        p->position.changePosition(m->velocity);
    }
}

void MovementSystem::update(float dt)
{
    std::vector<Entity*> validEntities = entityLayer->getEntitiesWithComponent<MovementComponent>();

    for (auto e : validEntities)
    {
        auto m = e->getComponent<MovementComponent>();

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
            (std::abs(m->velocity.x) > 0.001f) ? m->velocity.x *= game->getGamerules()->motion_friction : m->velocity.x = 0.f;
            (std::abs(m->velocity.y) > 0.001f) ? m->velocity.y *= game->getGamerules()->motion_friction : m->velocity.y = 0.f;
        }
    }

}