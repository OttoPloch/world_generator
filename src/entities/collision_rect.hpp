#pragma once

#include "../core/common.hpp"
#include "../world/world_position.hpp"
#include "rect_type.hpp"

struct CollisionRect
{
    CollisionRect();

    CollisionRect(WorldPosition position, sf::Vector2f size, RectType type);

    double left();
    double right();
    double top();
    double bottom();

    void setLeft(double x);
    void setRight(double x);
    void setTop(double y);
    void setBottom(double y);

    void move(sf::Vector2<double> dist);

    WorldPosition position;

    sf::Vector2f size;

    RectType type;
};