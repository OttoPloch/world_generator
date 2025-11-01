#include "motion_attribute.hpp"

MotionAttribute::MotionAttribute(sf::Vector2f position) : Attribute("motion"), position(position), velocity({0.f, 0.f}), rotationalVelocity(0.f)  {}

void MotionAttribute::tick()
{
    position.x += velocity.x;
    position.y += velocity.y;
}

void MotionAttribute::setVelocity(sf::Vector2f newVelocity)
{
    velocity = newVelocity;
}

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

void MotionAttribute::changeVelocity(sf::Vector2f amount)
{
    velocity += amount;
}

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

sf::Vector2f MotionAttribute::getPosition() { return position; }

float MotionAttribute::getRotation() { return rotation; }