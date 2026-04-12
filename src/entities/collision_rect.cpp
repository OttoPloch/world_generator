#include "collision_rect.hpp"

CollisionRect::CollisionRect() {}

CollisionRect::CollisionRect(WorldPosition position, sf::Vector2f size, RectType type) : position(position), size(size), type(type) {}

double CollisionRect::left()
{
    return position.getPos().x - size.x / 2.f;
}

double CollisionRect::right()
{
    return position.getPos().x + size.x / 2.f;
}

double CollisionRect::top()
{
    return position.getPos().y - size.y / 2.f;
}

double CollisionRect::bottom()
{
    return position.getPos().y + size.y / 2.f;
}

void CollisionRect::setLeft(double x)
{
    position.position->x = x + size.x / 2.f;
}

void CollisionRect::setRight(double x)
{
    position.position->x = x - size.x / 2.f;
}

void CollisionRect::setTop(double y)
{
    position.position->y = y + size.y / 2.f;
}

void CollisionRect::setBottom(double y)
{
    position.position->y = y - size.y / 2.f;
}

void CollisionRect::move(sf::Vector2<double> dist)
{
    position.position->x += dist.x;
    position.position->y += dist.y;
}