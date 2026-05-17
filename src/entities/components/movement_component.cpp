#include "movement_component.hpp"
#include "../../core/game.hpp"
#include "../entity.hpp"
#include "component_data.hpp"
#include "control_component.hpp"

MovementComponent::MovementComponent(Entity* myEntity, sf::Vector2f velocity, MovementComponentData stats) : EntityComponent(myEntity), velocity(velocity), stats(stats) {}

void MovementComponent::tick()
{
    myEntity->getPositionVar()->changePosition(velocity);
}

void MovementComponent::update(float dt)
{
    sf::Vector2f m = {0, 0};
    float speedMult = 1.f;

    if (myEntity->getComponent<ControlComponent>())
    {
        m = myEntity->game->getInput()->getMovement();

        if (myEntity->game->getInput()->isControlPressed("SPRINT")) speedMult = stats.sprintMultilpier;
    }

    if (m != sf::Vector2f(0, 0))
    {
        velocity.x = stats.speed * speedMult * m.x;
        velocity.y = stats.speed * speedMult * m.y;
    }
    else
    {
        (std::abs(velocity.x) > 0.001f) ? velocity.x *= myEntity->game->getGamerules()->motion_friction : velocity.x = 0.f;
        (std::abs(velocity.y) > 0.001f) ? velocity.y *= myEntity->game->getGamerules()->motion_friction : velocity.y = 0.f;
    }
}