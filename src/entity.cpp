#include "entity.hpp"

Entity::Entity() {}

void Entity::create(sf::Vector2f position)
{
    this->position = position;

    rotation = 0;
}

void Entity::giveSprite(std::string path, sf::Vector2f size)
{
    sprite.create(path, position, size);
}

void Entity::draw(sf::RenderWindow& window)
{
    sprite.draw(window);
}

sf::Vector2f Entity::getPosition() { return position; }