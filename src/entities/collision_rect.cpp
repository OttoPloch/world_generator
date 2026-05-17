#include "collision_rect.hpp"

CollisionRect::CollisionRect() : position(nullptr, {0, 0}, PositionType::WORLD) {}

CollisionRect::CollisionRect(GamePosition position, sf::Vector2f size, RectType type) : position(position), size(size), type(type) {}

float CollisionRect::left()
{
    return position.getPosition().x - size.x / 2.f;
}

float CollisionRect::right()
{
    return position.getPosition().x + size.x / 2.f;
}

float CollisionRect::top()
{
    return position.getPosition().y - size.y / 2.f;
}

float CollisionRect::bottom()
{
    return position.getPosition().y + size.y / 2.f;
}

void CollisionRect::setLeft(float x)
{
    position.setPosition('x', x + size.x / 2.f);
}

void CollisionRect::setRight(float x)
{
    position.setPosition('x', x - size.x / 2.f);
}

void CollisionRect::setTop(float y)
{
    position.setPosition('y', y + size.y / 2.f);
}

void CollisionRect::setBottom(float y)
{
    position.setPosition('y', y - size.y / 2.f);
}

void CollisionRect::move(sf::Vector2f dist)
{
    position.changePosition(dist);
}