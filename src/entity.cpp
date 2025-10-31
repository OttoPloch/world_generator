#include "entity.hpp"

Entity::Entity() {}

void Entity::create(sf::Vector2f position)
{
    this->position = position;

    rotation = 0;
}

void Entity::giveSprite(sf::Texture* texture, sf::Vector2f size, bool centerOrigin)
{
    sprite.create(texture, position, size, centerOrigin);
}

void Entity::changeSpriteTexture(sf::Texture* texture)
{
    sprite.setTexture(texture);
}

void Entity::draw(sf::RenderWindow& window)
{
    sprite.draw(window);
}

sf::Vector2f Entity::getPosition() { return position; }

const Sprite& Entity::getSprite() { return sprite; }