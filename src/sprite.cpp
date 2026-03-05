#include "sprite.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

Sprite::Sprite() {}

Sprite::Sprite(WorldPosition position, sf::Texture* texture, sf::Vector2f size, bool sizeIsScale, bool usingTexCoords, sf::IntRect texCoords)
{
    this->position = position;
    this->texture = texture;

    sprite = std::make_unique<sf::Sprite>(*texture);

    sprite->setPosition(position.getPos());

    if (usingTexCoords) sprite->setTextureRect(texCoords);

    if (sizeIsScale)
    {
        sprite->setScale(size);
        this->size = {sprite->getTextureRect().size.x * size.x, sprite->getTextureRect().size.y * size.y};
    }
    else
    {
        sprite->setScale({size.x / sprite->getTextureRect().size.x, size.y / sprite->getTextureRect().size.y});
        this->size = size;
    }

    sprite->setOrigin({sprite->getTextureRect().size.x / 2.f, sprite->getTextureRect().size.y / 2.f});
}

sf::Vector2f Sprite::getPosition() { return position.getPos(); }

sf::Vector2f Sprite::getSize() { return size; }

float Sprite::left() { return position.getPos().x - size.x / 2.f; }

float Sprite::right() { return position.getPos().x + size.x / 2.f; }

float Sprite::top() { return position.getPos().y - size.y / 2.f; }

float Sprite::bottom() { return position.getPos().y + size.y / 2.f; }

void Sprite::syncPos() { sprite->setPosition(position.getPos()); }

void Sprite::draw(sf::RenderWindow& window) { window.draw(*sprite.get()); }