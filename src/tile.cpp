#include "tile.hpp"
#include "game.hpp"

Tile::Tile() {}

Tile::Tile(sf::Vector2f position, sf::Vector2f size, int type)
{
    init(position, size, type);
}

void Tile::init(sf::Vector2f position, sf::Vector2f size, int type)
{
    this->position = position;

    this->size = size;

    this->type = type;
}

sf::Vector2f Tile::getPosition() { return position; }

sf::Vector2f Tile::getSize() { return size; }

int Tile::getType() { return type; }