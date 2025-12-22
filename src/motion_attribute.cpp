#include "motion_attribute.hpp"
#include "states.hpp"

MotionAttribute::MotionAttribute(EntityStates* states, GamePosition position, bool controlling) : Attribute("motion"), velocity({0.f, 0.f}), rotationalVelocity(0.f)
{
    this->states = states;

    this->position = position;

    this->controlling = controlling;
}

void MotionAttribute::tick()
{
    position.change(velocity);

    // TODO: put in container class
    float friction = 0.8f;
    float velocityCutoff = 0.01f;

    if (controlling)
    {
        sf::Vector2f movement = getMovement();

        // TODO: same with friction and cutoff above
        float speed = 7.5f;

        if (getKey("SHIFT")) speed *= 2;

        if (movement.x != 0)
        {
            velocity.x = movement.x * speed;

            states->set("animation", ANIM_MOVING);
        }
        else
        {
            (abs(velocity.x) > velocityCutoff) ? velocity.x *= friction : velocity.x = 0.f;
        }

        if (abs(velocity.y) <= abs(velocity.x))
        {
            if (velocity.x < 0) states->set("animation", ANIM_WALKINGLEFT, velocity.x);
            if (velocity.x > 0) states->set("animation", ANIM_WALKINGRIGHT, velocity.x);
        }

        if (movement.y != 0)
        {
            velocity.y = movement.y * speed;

            states->set("animation", ANIM_MOVING);
        }
        else
        {
            (abs(velocity.y) > velocityCutoff) ? velocity.y *= friction : velocity.y = 0.f;
        }

        if (velocity.y < 0) states->set("animation", ANIM_WALKINGUP, velocity.y);
        if (velocity.y > 0) states->set("animation", ANIM_WALKINGDOWN, velocity.y);
    }
    else
    {
        (abs(velocity.x) > velocityCutoff) ? velocity.x *= friction : velocity.x = 0.f;
        (abs(velocity.y) > velocityCutoff) ? velocity.y *= friction : velocity.y = 0.f;
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