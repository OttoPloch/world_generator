#include "collision_rect.hpp"

CollisionRect::CollisionRect() {}

CollisionRect::CollisionRect(WorldPosition position, sf::Vector2f size, RectType type) : position(position), size(size), type(type) {}

float CollisionRect::left()
{
    return position.getPos().x - size.x / 2.f;
}

float CollisionRect::right()
{
    return position.getPos().x + size.x / 2.f;
}

float CollisionRect::top()
{
    return position.getPos().y - size.y / 2.f;
}

float CollisionRect::bottom()
{
    return position.getPos().y + size.y / 2.f;
}

void CollisionRect::setLeft(float x)
{
    position.position->x = x + size.x / 2.f;
}

void CollisionRect::setRight(float x)
{
    position.position->x = x - size.x / 2.f;
}

void CollisionRect::setTop(float y)
{
    position.position->y = y + size.y / 2.f;
}

void CollisionRect::setBottom(float y)
{
    position.position->y = y - size.y / 2.f;
}

void CollisionRect::move(sf::Vector2f dist)
{
    position.position->x += dist.x;
    position.position->y += dist.y;
}