#pragma once

#include "../core/common.hpp"
#include "../utils/game_position.hpp"
#include "rect_type.hpp"

struct CollisionRect
{
    CollisionRect();

    CollisionRect(GamePosition position, sf::Vector2f size, RectType type);

    float left();
    float right();
    float top();
    float bottom();

    void setLeft(float x);
    void setRight(float x);
    void setTop(float y);
    void setBottom(float y);

    void move(sf::Vector2f dist);

    GamePosition position;

    sf::Vector2f size;

    RectType type;
};