#include "movement_component.hpp"
#include "control_component.hpp"
#include "entity.hpp"
#include "game.hpp"

MovementComponent::MovementComponent(Entity* myEntity, sf::Vector2f velocity, float speed) : EntityComponent(myEntity), velocity(velocity), speed(speed) {}

void MovementComponent::tick()
{
    sf::Vector2f* position = myEntity->getPositionVar()->position.get();

    position->x += velocity.x;
    position->y += velocity.y;
}

void MovementComponent::update()
{
    sf::Vector2f m = {0, 0};

    if (myEntity->getComponent<ControlComponent>()) m = myEntity->game->getInput()->getMovement();

    if (m != sf::Vector2f(0, 0))
    {
        velocity.x = speed * m.x;
        velocity.y = speed * m.y;
    }
    else
    {
        (std::abs(velocity.x) > 0.001f) ? velocity.x *= 0.8f : velocity.x = 0.f;
        (std::abs(velocity.y) > 0.001f) ? velocity.y *= 0.8f : velocity.y = 0.f;
    }
}