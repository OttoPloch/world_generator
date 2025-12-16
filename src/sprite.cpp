#include "sprite.hpp"
#include "animation.hpp"

Sprite::Sprite() {}

void Sprite::create(sf::Texture* texture, GamePosition position, sf::Vector2f size, bool centerOrigin)
{    
    this->position = position;

    spritePosition = position.get();
    
    this->size = size;

    rotation = 0.f;

    this->texture = texture;
    
    sprite = std::make_unique<sf::Sprite>(*texture);
    if (centerOrigin) centerSprite();
    sprite->setScale({size.x / sprite->getTextureRect().size.x, size.y / sprite->getTextureRect().size.y});
    sprite->setPosition(spritePosition);

    animation = nullptr;
}

void Sprite::centerSprite()
{
    sprite->setOrigin({sprite->getTextureRect().size.x / 2.f, sprite->getTextureRect().size.y / 2.f});
}

void Sprite::setSize(sf::Vector2f newSize)
{
    sprite->setScale({newSize.x / sprite->getTextureRect().size.x, newSize.y / sprite->getTextureRect().size.y});

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

void Sprite::giveAnimation(Animation* animation, unsigned int ticksPerFrame, bool reverse)
{
    this->animation = animation;

    animTicksPerFrame = ticksPerFrame;
    animReverse = reverse;

    animTicksToNextFrame = ticksPerFrame;
    animFrameIndex = 0;

    animPlaying = false;

    sprite->setTexture(*animation->getTexture());
    sprite->setTextureRect(sf::IntRect(animation->getFrameCoords(0), animation->getFrameSize()));
    centerSprite();
    setSize(size);
}

void Sprite::animPlay()
{
    animPlaying = true;
}

void Sprite::animStop()
{
    animPlaying = false;
}

void Sprite::animReset()
{
    animFrameIndex = 0;
    animTicksToNextFrame = animTicksPerFrame;
}

void Sprite::tick()
{
    if (animation)
    {
        if (animPlaying)
        {
            animTicksToNextFrame--;
    
            if (animTicksToNextFrame == 0)
            {
                animTicksToNextFrame = animTicksPerFrame;
        
                (animReverse) ? animFrameIndex-- : animFrameIndex++;
        
                if (animFrameIndex > animation->getFrameCount() - 1)
                {
                    animFrameIndex = 0;
                }
                if (animFrameIndex < 0)
                {
                    animFrameIndex = animation->getFrameCount() - 1;
                }

                sprite->setTextureRect(sf::IntRect(animation->getFrameCoords(animFrameIndex), animation->getFrameSize()));
            }
        }
    }
}

void Sprite::update(float dt)
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

float Sprite::getBottom() { return spritePosition.y + (size.y / 2.f); }

void Sprite::jumpToTarget()
{
    sprite->setPosition(position.get());
    spritePosition = position.get();
}