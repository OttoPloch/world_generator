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

    states.init();
}

int Entity::getID() { return ID; }

void Entity::giveSprite(sf::Texture* texture, sf::Vector2f size, int z, bool centerOrigin)
{
    sprite = std::make_unique<Sprite>();

    sf::Vector2f spriteSize;

    if (size == sf::Vector2f(-1.f, -1.f))
    {
        spriteSize = toV2F(texture->getSize());
    }
    else
    {
        spriteSize = size;
    }

    sprite->create(&states, texture, position, spriteSize, z, centerOrigin);
}

void Entity::giveMotion(bool controlling)
{
    motion = std::make_unique<MotionAttribute>(&states, position, controlling);
}

void Entity::giveCollision(std::vector<Entity>* entities, bool active, std::string name, sf::Vector2f offsetFraction, sf::Vector2f size, bool sizeIsFraction)
{
    sf::Vector2f offset = {sprite->getSize().x * offsetFraction.x, sprite->getSize().y * offsetFraction.y};
    
    sf::Vector2f collRectSize;

    if (sizeIsFraction)
    {
        collRectSize = {sprite->getSize().x * size.x, sprite->getSize().y * size.y};
    }
    else
    {
        collRectSize = size;
    }

    collision = std::make_unique<CollisionAttribute>(this, &states, position, offset, collRectSize, entities, active, name);
}

void Entity::changeSpriteTexture(sf::Texture* texture)
{
    sprite->setTexture(texture);
}

void Entity::tick()
{
    states.resetAll();

    if (motion) motion->tick();

    if (sprite) (collision && collision->active) ? sprite->tick() : sprite->tick();

    if (collision && collision->active == true) collision->tick();
}

void Entity::update(float dt)
{
    if (sprite) sprite->update(dt);
}

void Entity::draw(sf::RenderWindow& window)
{
    if (sprite) sprite->draw(window);
}

sf::Vector2f Entity::getPosition() { return *(position.position); }

Sprite* Entity::getSprite() { return sprite.get(); }

MotionAttribute* Entity::getMotion() { return motion.get(); }

CollisionAttribute* Entity::getCollision() { return collision.get(); }