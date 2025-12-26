#include "entity.hpp"
#include "collision_attribute.hpp"
#include "game.hpp"

Entity::Entity() {}

Entity::Entity(Game* game, int ID, sf::Vector2f position)
{
    create(game, ID, position);
}

void Entity::create(Game* game, int ID, sf::Vector2f position)
{
    this->game = game;

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

    sprite->create(game, this, texture, position, spriteSize, z, centerOrigin);
}

void Entity::giveMotion(float mass, bool controlling)
{
    motion = std::make_unique<MotionAttribute>(game, this, position, mass, controlling);
}

void Entity::giveCollision(std::vector<Entity>* entities, std::string name, int rectType, std::vector<std::string> blacklist, sf::Vector2f offsetFraction, sf::Vector2f size, bool sizeIsFraction)
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

    collision = std::make_unique<CollisionAttribute>(this, position, offset, collRectSize, entities, name, rectType, blacklist);
}

void Entity::changeSpriteTexture(sf::Texture* texture)
{
    sprite->setTexture(texture);
}

void Entity::tick()
{
    states.resetAll();

    if (motion) motion->tick();

    if (collision) collision->tick();
    
    if (sprite) sprite->tick();
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

EntityStates* Entity::getStates() { return &states; }

Sprite* Entity::getSprite() { return sprite.get(); }

MotionAttribute* Entity::getMotion() { return motion.get(); }

CollisionAttribute* Entity::getCollision() { return collision.get(); }