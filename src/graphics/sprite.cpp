#include "sprite.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

Sprite::Sprite() : position(nullptr, {0, 0}) {}

Sprite::Sprite(GamePosition position, sf::Texture* texture, sf::Vector2f size, bool sizeIsScale, bool usingTexCoords, sf::IntRect texCoords, float animSpeedMult) : position(position), texture(texture)
{
    sprite = std::make_unique<sf::Sprite>(*texture);
    
    sprite->setPosition(position.getPosition());
    
    if (usingTexCoords) sprite->setTextureRect(texCoords);
    
    this->animSpeedMult = animSpeedMult;

    resize(size, sizeIsScale);
}

sf::Vector2f Sprite::getPosition() { return position.getPosition(); }

sf::Vector2f Sprite::getSize() { return size; }

float Sprite::left() { return position.getPosition().x - size.x / 2.f; }

float Sprite::right() { return position.getPosition().x + size.x / 2.f; }

float Sprite::top() { return position.getPosition().y - size.y / 2.f; }

float Sprite::bottom() { return position.getPosition().y + size.y / 2.f; }

void Sprite::resize(sf::Vector2f newSize, bool sizeIsScale)
{
    if (sizeIsScale)
    {
        sprite->setScale(newSize);
        this->size = {sprite->getTextureRect().size.x * newSize.x, sprite->getTextureRect().size.y * newSize.y};
    }
    else
    {
        sprite->setScale({newSize.x / sprite->getTextureRect().size.x, newSize.y / sprite->getTextureRect().size.y});
        this->size = newSize;
    }

    sprite->setOrigin({sprite->getTextureRect().size.x / 2.f, sprite->getTextureRect().size.y / 2.f});
}

void Sprite::setTextureRect(sf::IntRect newTexRect)
{
    sprite->setTextureRect(newTexRect);

    resize(size, false);
}

void Sprite::syncPos(sf::Vector2f interpolatedPos, bool useInterpolated)
{
    if (useInterpolated)
    {
        sprite->setPosition(interpolatedPos);
    }
    else
    {
        sprite->setPosition(position.getPosition());
    }
}

void Sprite::update(float dt)
{
    Animation* activeAnimation = nullptr;

    if (animation.get()) activeAnimation = animation.get();
    else if (animSet.get()) activeAnimation = animSet->getActiveAnimation();

    if (activeAnimation)
    {
        activeAnimation->secondsTillNextFrame -= dt * animSpeedMult;

        if (activeAnimation->secondsTillNextFrame <= 0.f)
        {
            (activeAnimation->reversed) ? activeAnimation->index-- : activeAnimation->index++;
            
            if (activeAnimation->index >= activeAnimation->frames.size()) activeAnimation->index = 0;
            if (activeAnimation->index < 0) activeAnimation->index = activeAnimation->frames.size() - 1;
            
            // TODO: devise a better way to check if a sprite and its animation have the same texture,
            // without using getNativeHandle.
            if (activeAnimation->texture->getNativeHandle() != sprite->getTexture().getNativeHandle())
            {
                std::cout << "ACTIVE ANIMATION TEXTURE AND SPRITE CURRENT TEXTURE ARE DIFFERENT, UPDATING SPRITE TEXTURE.\n";
                sprite->setTexture(*activeAnimation->texture);
            }

            setTextureRect(activeAnimation->frames[activeAnimation->index]);

            activeAnimation->secondsTillNextFrame = activeAnimation->secondsPerFrame;
        }
    }
}

void Sprite::draw(sf::RenderWindow& window) { window.draw(*sprite.get()); }