#include "motion_attribute.hpp"

MotionAttribute::MotionAttribute(EntityStates* states, GamePosition position, bool controlling) : Attribute("motion"), velocity({0.f, 0.f}), rotationalVelocity(0.f)
{
    this->states = states;

    this->position = position;

    this->controlling = controlling;
}

void MotionAttribute::tick()
{
    position.change(velocity);

    if (controlling)
    {
        sf::Vector2f movement = getMovement();

        float speed = 7.5f;

        if (getKey("SHIFT")) speed *= 2;

        if (movement.x != 0)
        {
            velocity.x = movement.x * speed;

            states->set("walking");
        }
        else
        {
            if (velocity.x != 0.f)
            {
                velocity.x *= 0.1f;
                
                if (abs(velocity.x) < 0.01f) velocity.x = 0.f;
            }
        }

        if (abs(velocity.y) <= abs(velocity.x))
        {
            if (velocity.x < 0) states->set("walkingLeft", velocity.x);
            if (velocity.x > 0) states->set("walkingRight", velocity.x);
        }

        if (movement.y != 0)
        {
            velocity.y = movement.y * speed;

            states->set("walking");
        }
        else
        {
            if (velocity.y != 0.f)
            {
                velocity.y *= 0.1f;
                
                if (abs(velocity.y) < 0.01f) velocity.y = 0.f;
            }
        }

        if (velocity.y < 0) states->set("walkingUp", velocity.y);
        if (velocity.y > 0) states->set("walkingDown", velocity.y);
    }
    else
    {
        if (velocity.x != 0.f) velocity.x *= 0.1f;
        if (velocity.y != 0.f) velocity.y *= 0.1f;
    }
}

sf::Vector2f MotionAttribute::getVelocity() { return velocity; }

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