#pragma once

#include "common.hpp"
#include "attribute.hpp"

class MotionAttribute : public Attribute
{
public:
    MotionAttribute(sf::Vector2f position);

    void tick();

    void setVelocity(sf::Vector2f newVelocity);

    void setVelocity(char direction, float newVelocity);
    
    void changeVelocity(sf::Vector2f amount);

    void changeVelocity(char direction, float amount);

    sf::Vector2f getPosition();

    float getRotation();
private:
    sf::Vector2f position;
    float rotation;

    sf::Vector2f velocity;

    float rotationalVelocity;
};