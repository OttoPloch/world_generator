#include "collision_rect.hpp"

#include <algorithm>

CollisionRect::CollisionRect() {}

void CollisionRect::init(GamePosition position, sf::Vector2f offset, sf::Vector2f size, std::string colliderName, RectType rectType, std::vector<std::string> blacklist)
{
    this->position = position;
    this->offset = offset;
    this->size = size;
    
    defaultOffset = offset;
    defaultSize = size;

    offsetPosition = {position.get().x + offset.x, position.get().y + offset.y};

    lastPosition = position.get();

    this->colliderName = colliderName;

    this->rectType = rectType;

    this->blacklist = blacklist;
}

void CollisionRect::updatePosition()
{
    offsetPosition = {position.get().x + offset.x, position.get().y + offset.y};
}

sf::Vector2f CollisionRect::center() { return offsetPosition; }

sf::Vector2f CollisionRect::getSize() { return size; }

void CollisionRect::setSize(sf::Vector2f size)
{
    this->size = size;
}

std::array<float, 4> CollisionRect::getBorders()
{
    float left = offsetPosition.x - (size.x / 2.f);
    float right = offsetPosition.x + (size.x / 2.f);
    float top = offsetPosition.y - (size.y / 2.f);
    float bottom = offsetPosition.y + (size.y / 2.f);

    return {left, right, top, bottom};
}

float CollisionRect::left()
{
    return offsetPosition.x - (size.x / 2.f);
}

float CollisionRect::right()
{
    return offsetPosition.x + (size.x / 2.f);
}

float CollisionRect::top()
{
    return offsetPosition.y - (size.y / 2.f);
}

float CollisionRect::bottom()
{
    return offsetPosition.y + (size.y / 2.f);
}

float CollisionRect::length()
{
    return size.x;
}

float CollisionRect::halfLength()
{
    return size.x / 2.f;    
}

float CollisionRect::width()
{
    return size.y;
}

float CollisionRect::halfWidth()
{
    return size.y / 2.f;
}

void CollisionRect::setBasePosition(sf::Vector2f newPosition)
{
    position.set(newPosition);

    updatePosition();
}

void CollisionRect::setOffsetPosition(sf::Vector2f newPosition)
{
    offsetPosition = newPosition;

    position.set({offsetPosition.x - offset.x, offsetPosition.y - offset.y});
}

void CollisionRect::setOffset(sf::Vector2f newOffset)
{
    offset = newOffset;

    updatePosition();
}

void CollisionRect::setLeft(float value)
{
    offsetPosition.x = value + (size.x / 2.f);

    position.set('x', offsetPosition.x - offset.x);
}

void CollisionRect::setRight(float value)
{
    offsetPosition.x = value - (size.x / 2.f);
    
    position.set('x', offsetPosition.x - offset.x);
}

void CollisionRect::setTop(float value)
{
    offsetPosition.y = value + (size.y / 2.f);

    position.set('y', offsetPosition.y - offset.y);
}

void CollisionRect::setBottom(float value)
{
    offsetPosition.y = value - (size.y / 2.f);

    position.set('y', offsetPosition.y - offset.y);
}

void CollisionRect::setToDefault()
{
    setOffset(defaultOffset);
    setSize(defaultSize);
}

std::string CollisionRect::getColliderName() { return colliderName; }

RectType CollisionRect::getType() { return rectType; }

void CollisionRect::setBlacklist(std::vector<std::string> newList)
{
    blacklist = newList;
}

void CollisionRect::addToBlacklist(std::string newEntry)
{
    blacklist.push_back(newEntry);
}

bool CollisionRect::searchBlacklist(std::string entry)
{
    return std::find(blacklist.begin(), blacklist.end(), entry) != blacklist.end();
}