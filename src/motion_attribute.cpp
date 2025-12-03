#include "motion_attribute.hpp"

MotionAttribute::MotionAttribute(GamePosition position, bool controlling) : Attribute("motion"), velocity({0.f, 0.f}), rotationalVelocity(0.f)
{
    this->position = position;

    this->controlling = controlling;
}

void MotionAttribute::tick()
{
    position.change(velocity);

    if (controlling)
    {
        sf::Vector2f movement = getMovement();

        (movement.x != 0.f) ? velocity.x = movement.x * 30.f : velocity.x *= 0.6f;
        (movement.y != 0.f) ? velocity.y = movement.y * 30.f : velocity.y *= 0.6f;
    }
    else
    {
        velocity.x *= 0.6f;
        velocity.y *= 0.6f;
    }
}

void MotionAttribute::setVelocity(sf::Vector2f newVelocity) { velocity = newVelocity; }

void MotionAttribute::setVelocity(char direction, float newVelocity)
{
    if (direction == 'x')
    {
        velocity.x = newVelocity;
    }
    else if (direction == 'y')
    {
        velocity.y = newVelocity;
    }
    else
    {
        std::cout << "pick a direction!\n";
        assert(false);
    }
}

void MotionAttribute::changeVelocity(sf::Vector2f amount) { velocity += amount; }

void MotionAttribute::changeVelocity(char direction, float amount)
{
    if (direction == 'x')
    {
        velocity.x += amount;
    }
    else if (direction == 'y')
    {
        velocity.y += amount;
    }
    else
    {
        std::cout << "pick a direction!\n";
        assert(false);
    }
}

float MotionAttribute::getRotation() { return rotation; }