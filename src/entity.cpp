#include "entity.hpp"

Entity::Entity() {}

void Entity::create(sf::Vector2f position)
{
    this->position = position;

    rotation = 0;
}

void Entity::giveSprite(sf::Texture* texture, sf::Vector2f size, bool centerOrigin)
{
    sprite = std::make_unique<Sprite>();

    sprite->create(texture, position, size, centerOrigin);
}

void Entity::giveMotion()
{
    motion = std::make_unique<MotionAttribute>(position);

    motion->setVelocity('x', 3.f);
    motion->setVelocity('y', 6.f);
}

void Entity::changeSpriteTexture(sf::Texture* texture)
{
    sprite->setTexture(texture);
}

void Entity::tick()
{
    if (motion)
    {
        motion->tick();

        position = motion->getPosition();
        
        if (sprite)
        {
            sprite->setPosition(position);
        }
    }

}

void Entity::draw(sf::RenderWindow& window)
{
    if (sprite) sprite->draw(window);
}

sf::Vector2f Entity::getPosition() { return position; }

const Sprite& Entity::getSprite() { return *sprite; }