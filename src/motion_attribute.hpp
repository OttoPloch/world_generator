#pragma once

#include "common.hpp"
#include "attribute.hpp"
#include "game_position.hpp"

class MotionAttribute : public Attribute
{
public:
    MotionAttribute(GamePosition position);

    void tick();

    void setVelocity(sf::Vector2f newVelocity);

    void setVelocity(char direction, float newVelocity);
    
    void changeVelocity(sf::Vector2f amount);

    void changeVelocity(char direction, float amount);
    
    float getRotation();
private:
    GamePosition position;

    float rotation;

    sf::Vector2f velocity;

    float rotationalVelocity;
};