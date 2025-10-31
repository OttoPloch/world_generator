#include "sprite.hpp"

Sprite::Sprite() {}

void Sprite::create(sf::Texture* texture, sf::Vector2f position, sf::Vector2f size, bool centerOrigin)
{
    this->position = position;
    
    this->size = size;

    rotation = 0.f;

    this->texture = texture;

    sprite = std::make_unique<sf::Sprite>(*texture);

    if (centerOrigin) sprite->setOrigin({sprite->getTextureRect().size.x / 2.f, sprite->getTextureRect().size.y / 2.f});
    sprite->setScale({size.x / sprite->getTextureRect().size.x, size.y / sprite->getTextureRect().size.y});
    sprite->setPosition(position);
}

void Sprite::setPosition(sf::Vector2f newPosition)
{
    sprite->setPosition(newPosition);

    position = newPosition;
}

void Sprite::setSize(sf::Vector2f newSize)
{
    sprite->setScale({newSize.x / size.x, newSize.y / size.y});

    size = newSize;
}

void Sprite::setRotation(float newRotation)
{
    sprite->setRotation(sf::degrees(newRotation));

    rotation = newRotation;
}

void Sprite::setTexture(sf::Texture* newTexture)
{
    sprite->setTexture(*newTexture);
    
    texture = newTexture;
}

void Sprite::draw(sf::RenderWindow& window)
{
    window.draw(*sprite);
}

sf::Sprite Sprite::getSprite() { return *sprite; }