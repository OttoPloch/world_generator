#include "sprite.hpp"

Sprite::Sprite() {}

void Sprite::create(sf::Texture* texture, GamePosition position, sf::Vector2f size, bool centerOrigin)
{    
    this->position = position;

    spritePosition = position.get();
    
    this->size = size;

    rotation = 0.f;

    this->texture = texture;
    
    sprite = std::make_unique<sf::Sprite>(*texture);
    if (centerOrigin) sprite->setOrigin({sprite->getTextureRect().size.x / 2.f, sprite->getTextureRect().size.y / 2.f});
    sprite->setScale({size.x / sprite->getTextureRect().size.x, size.y / sprite->getTextureRect().size.y});
    sprite->setPosition(spritePosition);
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

void Sprite::tick()
{

}

void Sprite::update(float dt, MotionAttribute* a)
{
    if (spritePosition != position.get())
    {
        float xDiff = position.get().x - spritePosition.x;
        float yDiff = position.get().y - spritePosition.y;

        // TODO: find a place for the constant determining delay, need some container
        // for these values since there can be many sprites, entities, etc. that would
        // all have their own memory for this constant if it is made a variable and left
        // in here. A gamerule class or something would be great for easy access to a lot
        // of fun stuff.

        float delay;
        if (.03f / dt >= 1.f)
        {
            delay = .03f / dt;
        }
        else
        {
            delay = 1.f;
        }

        (abs(xDiff) < 0.001f) ? spritePosition.x = position.get().x : spritePosition.x += xDiff / delay;
        (abs(yDiff) < 0.001f) ? spritePosition.y = position.get().y : spritePosition.y += yDiff / delay;
    }
    
    sprite->setPosition(spritePosition);
}

void Sprite::draw(sf::RenderWindow& window)
{
    window.draw(*sprite);
}

sf::Sprite Sprite::getSprite() { return *sprite; }

sf::Vector2f Sprite::getSpritePosition() { return spritePosition; }

sf::Vector2f Sprite::getSize() { return size; }

void Sprite::jumpToTarget()
{
    sprite->setPosition(position.get());
    spritePosition = position.get();
}