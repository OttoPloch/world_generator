#include "motion_attribute.hpp"
#include "states.hpp"
#include "game.hpp"

MotionAttribute::MotionAttribute(Game* game, Entity* myEntity, GamePosition position, float mass, bool controlling) : Attribute("motion"), velocity({0.f, 0.f}), rotationalVelocity(0.f)
{
    this->game = game;

    this->gamerules = game->getGamerules();

    states = myEntity->getStates();

    this->position = position;

    this->mass = mass;

    this->controlling = controlling;

    posChange = {0.f, 0.f};
}

void MotionAttribute::tick()
{
    posChange = velocity;
    // position.change(velocity);

    float friction = gamerules->getRule("motion_friction").valueFloat;
    float velocityCutoff = gamerules->getRule("motion_velocityCutoff").valueFloat;

    if (controlling)
    {   
        sf::Vector2f movement = game->getInput()->getMovement();

        // TODO: replace "player" here with
        // a variable representing the entity's name or type
        float baseSpeed = gamerules->getRule("moveSpeed", "player").valueFloat;
        float speed = baseSpeed;

        if (game->getInput()->getControl("SPRINT")) speed *= 2.f;

        if (movement.x != 0)
        {
            velocity.x = movement.x * speed;

            states->set("animation", ANIM_MOVING);
        }
        else
        {
            (std::fabs(velocity.x) > velocityCutoff) ? velocity.x *= friction : velocity.x = 0.f;
        }

        if (std::fabs(velocity.y) <= std::fabs(velocity.x))
        {
            if (game->getInput()->getControl("SPRINT"))
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
            (std::fabs(velocity.y) > velocityCutoff) ? velocity.y *= friction : velocity.y = 0.f;
        }

        if (game->getInput()->getControl("SPRINT"))
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
        (std::fabs(velocity.x) > velocityCutoff) ? velocity.x *= friction : velocity.x = 0.f;
        (std::fabs(velocity.y) > velocityCutoff) ? velocity.y *= friction : velocity.y = 0.f;
    }
}

void MotionAttribute::updateAxis(char axis)
{
    if (axis == 'x') position.change('x', posChange.x);
    if (axis == 'y') position.change('y', posChange.y);
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

float MotionAttribute::getMass() { return mass; }