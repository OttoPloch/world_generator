#include "entity.hpp"
#include "game.hpp"
#include "world_position.hpp"

Entity::Entity() {}

Entity::Entity(Game* game, int ID, sf::Vector2f position) : game(game), ID(ID)
{
    this->position = WorldPosition(position);

    sprite = Sprite(this->position, game->getAssetManager()->getTexture("bush"));
}

Entity::Entity(Game* game, int ID, WorldPosition position) : game(game), ID(ID), position(position) {}

Sprite* Entity::spriteInit(sf::Texture* texture, sf::Vector2f size, bool sizeIsScale, bool usingTexCoords, sf::IntRect texCoords, float animSpeedMult)
{
    sprite = Sprite(this->position, texture, size, sizeIsScale, usingTexCoords, texCoords, animSpeedMult);

    return &sprite;
}

int Entity::getID() { return ID; }

void Entity::tick()
{
    lastPosition = position.getPos();

    for (auto& c : components) c->tick();
}

void Entity::update(float dt)
{
    sprite.update(dt);

    for (auto& c : components) c->update();
}

void Entity::draw(float alpha, sf::RenderWindow& window)
{
    sf::Vector2f renderPos = position.getPos() * alpha + lastPosition * (1.f - alpha);

    sprite.syncPos(renderPos);

    sprite.draw(window);
}

sf::Vector2f Entity::getPosition() { return position.getPos(); }

WorldPosition* Entity::getPositionVar() { return &position; }

Sprite* Entity::getSprite() { return &sprite; }