#include "entity.hpp"

Entity::Entity() {}

void Entity::create(sf::Vector2f position)
{
    this->position.set(position);

    rotation = 0;
}

void Entity::giveSprite(sf::Texture* texture, sf::Vector2f size, bool centerOrigin)
{
    sprite = std::make_unique<Sprite>();

    sprite->create(texture, position, size, centerOrigin);
}

void Entity::giveMotion(bool controlling)
{
    motion = std::make_unique<MotionAttribute>(position, controlling);

    motion->setVelocity('x', 4.f);
    motion->setVelocity('y', 8.f);
}

void Entity::changeSpriteTexture(sf::Texture* texture)
{
    sprite->setTexture(texture);
}

void Entity::tick()
{
    if (motion) motion->tick();
    
    if (sprite) sprite->tick();
}

void Entity::draw(sf::RenderWindow& window)
{
    if (sprite) sprite->draw(window);
}

sf::Vector2f Entity::getPosition() { return *(position.position); }

const Sprite& Entity::getSprite() { return *sprite; }