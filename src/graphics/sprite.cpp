#include "sprite.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

Sprite::Sprite() : position(nullptr, {0, 0}) {}

Sprite::Sprite(GamePosition position, sf::Texture* texture, sf::Vector2f size, bool sizeIsScale, bool usingTexCoords, sf::IntRect texCoords, float animSpeedMult, bool repeatAnimation) : position(position), texture(texture), animSpeedMult(animSpeedMult), repeatAnimation(repeatAnimation)
{
    if (!texture)
    {
        std::cerr << "ERROR: cannot create sprite, no texture was given. Position: " << position.getPosition().x << ", " << position.getPosition().y << ". Size: " << size.x << ", " << size.y << '\n';
    }
    
    sprite = std::make_unique<sf::Sprite>(*texture);
    
    sprite->setPosition(position.getPosition());
    
    if (usingTexCoords) sprite->setTextureRect(texCoords);

    resize(size, sizeIsScale);
}

sf::Vector2f Sprite::getPosition() { return position.getPosition(); }

sf::Vector2f Sprite::getSize() { return size; }

GamePosition Sprite::getPositionVar() { return position; }

float Sprite::left() { return position.getPosition().x - size.x / 2.f; }

float Sprite::right() { return position.getPosition().x + size.x / 2.f; }

float Sprite::top() { return position.getPosition().y - size.y / 2.f; }

float Sprite::bottom() { return position.getPosition().y + size.y / 2.f; }

void Sprite::setTexture(sf::Texture* newTexture, bool resetTextureRect)
{
    if (!sprite) return;

    texture = newTexture;
    
    sprite->setTexture(*newTexture);

    if (resetTextureRect)
    {
        sprite->setTextureRect({{0, 0}, static_cast<sf::Vector2i>(texture->getSize())});
    }
}

void Sprite::resize(sf::Vector2f newSize, bool sizeIsScale)
{
    if (!sprite) return;

    this->sizeIsScale = sizeIsScale;

    if (sizeIsScale)
    {
        sprite->setScale(newSize);
        size = {sprite->getTextureRect().size.x * newSize.x, sprite->getTextureRect().size.y * newSize.y};
    }
    else
    {
        sprite->setScale({newSize.x / sprite->getTextureRect().size.x, newSize.y / sprite->getTextureRect().size.y});
        size = newSize;
    }

    sprite->setOrigin({sprite->getTextureRect().size.x / 2.f, sprite->getTextureRect().size.y / 2.f});
}

void Sprite::setTextureRect(sf::IntRect newTexRect)
{
    if (!sprite) return;

    sprite->setTextureRect(newTexRect);

    resize(size, sizeIsScale);
}

void Sprite::syncPos()
{
    if (!sprite) return;

    sprite->setPosition(position.getPosition());
}

void Sprite::update(float dt)
{
    if (!sprite) return;

    Animation* activeAnimation = nullptr;

    if (animation)
    {
        activeAnimation = animation.get();
    }
    else if (animSet)
    {
        activeAnimation = animSet->getActiveAnimation();
    }

    if (activeAnimation)
    {
        activeAnimation->secondsTillNextFrame -= dt * animSpeedMult;

        while (activeAnimation->secondsTillNextFrame <= 0.f)
        {
            (activeAnimation->reversed) ? activeAnimation->index-- : activeAnimation->index++;
            
            if (repeatAnimation)
            {
                if (activeAnimation->index >= activeAnimation->frames.size()) activeAnimation->index = 0;
                if (activeAnimation->index < 0) activeAnimation->index = activeAnimation->frames.size() - 1;
            }
            else
            {
                if (activeAnimation->index >= activeAnimation->frames.size()) activeAnimation->index = activeAnimation->frames.size() - 1;
                if (activeAnimation->index < 0) activeAnimation->index = 0;
            }
            
            // TODO: devise a better way to check if a sprite and its animation have the same texture,
            // without using getNativeHandle.
            if (activeAnimation->texture->getNativeHandle() != sprite->getTexture().getNativeHandle())
            {
                std::cout << "ACTIVE ANIMATION TEXTURE AND SPRITE CURRENT TEXTURE ARE DIFFERENT, UPDATING SPRITE TEXTURE.\n";
                sprite->setTexture(*activeAnimation->texture);
            }
            
            activeAnimation->secondsTillNextFrame += activeAnimation->secondsPerFrame;
        }
        
        setTextureRect({
            toV2I(activeAnimation->frames[activeAnimation->index].position),
            toV2I(activeAnimation->frames[activeAnimation->index].size)
        });
    }
}

void Sprite::draw(sf::RenderWindow& window)
{
    if (!sprite) return;

    window.draw(*sprite);
}