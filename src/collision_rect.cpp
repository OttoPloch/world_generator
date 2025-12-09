#include "collision_rect.hpp"

CollisionRect::CollisionRect() {}

void CollisionRect::init(GamePosition position, sf::Vector2f size)
{
    this->position = position;
    this->size = size;
}

sf::Vector2f CollisionRect::center() { return position.get(); }

sf::Vector2f CollisionRect::getSize() { return size; }

std::array<float, 4> CollisionRect::getBorders()
{
    float left = position.get().x - (size.x / 2.f);
    float right = position.get().x + (size.x / 2.f);
    float top = position.get().y - (size.y / 2.f);
    float bottom = position.get().y + (size.y / 2.f);

    return {left, right, top, bottom};
}

float CollisionRect::left()
{
    return position.get().x - (size.x / 2.f);
}

float CollisionRect::right()
{
    return position.get().x + (size.x / 2.f);
}

float CollisionRect::top()
{
    return position.get().y - (size.y / 2.f);;
}

float CollisionRect::bottom()
{
    return position.get().y + (size.y / 2.f);
}

void CollisionRect::setLeft(float value)
{
    position.set('x', value + (size.x / 2.f));
}

void CollisionRect::setRight(float value)
{
    position.set('x', value - (size.x / 2.f));
}

void CollisionRect::setTop(float value)
{
    position.set('y', value + (size.y / 2.f));
}

void CollisionRect::setBottom(float value)
{
    position.set('y', value - (size.y / 2.f));
}