#pragma once

#include "../core/common.hpp"
#include "../world/world_position.hpp"
#include "rect_type.hpp"

struct CollisionRect
{
    CollisionRect();

    CollisionRect(WorldPosition position, sf::Vector2f size, RectType type);

    float left();
    float right();
    float top();
    float bottom();

    void setLeft(float x);
    void setRight(float x);
    void setTop(float y);
    void setBottom(float y);

    void move(sf::Vector2f dist);

    WorldPosition position;

    sf::Vector2f size;

    RectType type;
};