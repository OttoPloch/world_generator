#include "tile.hpp"

Tile::Tile() {}

Tile::Tile(sf::Vector2f position, sf::Vector2f size, int type, bool collides, std::string colliderName, sf::Vector2f collOffsetFraction, sf::Vector2f collSizeFraction)
{
    init(position, size, type, collides, colliderName, collOffsetFraction, collSizeFraction);
}

void Tile::init(sf::Vector2f position, sf::Vector2f size, int type, bool collides, std::string colliderName, sf::Vector2f collOffsetFraction, sf::Vector2f collSizeFraction)
{
    this->position = position;

    this->size = size;

    this->type = type;

    this->collides = collides;

    this->colliderName = colliderName;
    
    this->collOffsetFraction = collOffsetFraction;
    
    this->collSizeFraction = collSizeFraction;
}

sf::Vector2f Tile::getPosition() { return position; }

sf::Vector2f Tile::getSize() { return size; }

int Tile::getType() { return type; }

bool Tile::hasCollider() { return collides; }

sf::FloatRect Tile::getRect()
{
    if (collides)
    {
        sf::Vector2f collSize = {size.x * collSizeFraction.x, size.y * collSizeFraction.y};

        return sf::FloatRect({(position.x + size.x / 2.f - collSize.x / 2.f) + size.x * collOffsetFraction.x, (position.y + size.y / 2.f - collSize.y / 2.f) + size.y * collOffsetFraction.y}, collSize);
    }
    else
    {
        return sf::FloatRect({0.f, 0.f}, {-1.f, -1.f});
    }
}

std::string Tile::getColliderName() { return colliderName; }