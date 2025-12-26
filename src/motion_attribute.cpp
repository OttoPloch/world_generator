#include "motion_attribute.hpp"
#include "states.hpp"
#include "game.hpp"

MotionAttribute::MotionAttribute(Game* game, EntityStates* states, GamePosition position, bool controlling) : Attribute("motion"), velocity({0.f, 0.f}), rotationalVelocity(0.f)
{
    this->gamerules = game->getGamerules();

    this->states = states;

    this->position = position;

    this->controlling = controlling;
}

void MotionAttribute::tick()
{
    position.change(velocity);

    float friction = gamerules->getRule("motion_friction").valueFloat;
    float velocityCutoff = gamerules->getRule("motion_velocityCutoff").valueFloat;

    if (controlling)
    {
        sf::Vector2f movement = getMovement();

        float baseSpeed = gamerules->getRule("moveSpeed", "player").valueFloat;
        float speed = baseSpeed;

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
            if (getKey("SHIFT"))
            {
                if (velocity.x < 0) states->set("animation", ANIM_RUNNINGLEFT, (baseSpeed / speed));
                if (velocity.x > 0) states->set("animation", ANIM_RUNNINGRIGHT, .8f);
            }
            else
            {
                if (velocity.x < 0) states->set("animation", ANIM_WALKINGLEFT, (baseSpeed / speed));
                if (velocity.x > 0) states->set("animation", ANIM_WALKINGRIGHT, (baseSpeed / speed));
            }
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

        if (getKey("SHIFT"))
        {
            if (velocity.y < 0) states->set("animation", ANIM_RUNNINGUP, (baseSpeed / speed));
            if (velocity.y > 0) states->set("animation", ANIM_RUNNINGDOWN, (baseSpeed / speed));
        }
        else
        {
            if (velocity.y < 0) states->set("animation", ANIM_WALKINGUP, (baseSpeed / speed));
            if (velocity.y > 0) states->set("animation", ANIM_WALKINGDOWN, (baseSpeed / speed));
        }
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