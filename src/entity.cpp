#include "entity.hpp"
#include "collision_attribute.hpp"

Entity::Entity() {}

Entity::Entity(int ID, sf::Vector2f position)
{
    create(ID, position);
}

void Entity::create(int ID, sf::Vector2f position)
{
    this->ID = ID;

    this->position.set(position);

    rotation = 0;
}

int Entity::getID() { return ID; }

void Entity::giveSprite(sf::Texture* texture, sf::Vector2f size, bool centerOrigin)
{
    sprite = std::make_unique<Sprite>();

    sprite->create(texture, position, size, centerOrigin);
}

void Entity::giveMotion(bool controlling)
{
    motion = std::make_unique<MotionAttribute>(position, controlling);
}

void Entity::giveCollision(std::vector<Entity>* entities, bool active)
{
    collision = std::make_unique<CollisionAttribute>(this, position, sprite->getSize(), entities, active);
}

void Entity::changeSpriteTexture(sf::Texture* texture)
{
    sprite->setTexture(texture);
}

void Entity::tick()
{
    if (motion) motion->tick();
    
    if (sprite) sprite->tick();

    if (collision && collision->active == true) collision->tick();
}

void Entity::update(float dt)
{
    if (sprite) sprite->update(dt, motion.get());
}

void Entity::draw(sf::RenderWindow& window)
{
    if (sprite) sprite->draw(window);
}

sf::Vector2f Entity::getPosition() { return *(position.position); }

Sprite* Entity::getSprite() { return sprite.get(); }

MotionAttribute* Entity::getMotion() { return motion.get(); }

CollisionAttribute* Entity::getCollision() { return collision.get(); }