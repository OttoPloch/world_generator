#pragma once

#include "common.hpp"
#include "game_position.hpp"

#include <array>

class CollisionRect
{
public:
    CollisionRect();

    void init(GamePosition position, sf::Vector2f offset, sf::Vector2f size);

    void updatePosition();

    sf::Vector2f center();
    
    sf::Vector2f getSize();
    
    // left, right, top, bottom
    std::array<float, 4> getBorders();

    float left();

    float right();

    float top();

    float bottom();

    float length();
    
    float halfLength();
    
    float width();

    float halfWidth();

    void setBasePosition(sf::Vector2f newPosition);

    void setOffsetPosition(sf::Vector2f newPosition);

    void setLeft(float value);

    void setRight(float value);

    void setTop(float value);

    void setBottom(float value);
    
    sf::Vector2f lastPosition;
private:
    GamePosition position;

    sf::Vector2f offset;

    sf::Vector2f size;
    
    sf::Vector2f offsetPosition;
};