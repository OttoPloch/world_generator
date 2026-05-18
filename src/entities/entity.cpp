#include "entity.hpp"
#include "../core/game.hpp"
#include "components/collision_component.hpp"

Entity::Entity() : position(nullptr, {0, 0}) {}

Entity::Entity(Game* game, int ID, GamePosition position) : game(game), ID(ID), position(position) {}

Sprite* Entity::spriteInit(sf::Texture* texture, sf::Vector2f size, bool sizeIsScale, bool usingTexCoords, sf::IntRect texCoords, float animSpeedMult)
{
    sprite = Sprite(position, texture, size, sizeIsScale, usingTexCoords, texCoords, animSpeedMult);

    return &sprite;
}

int Entity::getID() { return ID; }

void Entity::tick()
{
    lastPosition = position.getPosition();

    for (auto& c : components) c->tick();
}

void Entity::update(float dt)
{
    sprite.update(dt);

    for (auto& c : components) c->update(dt);
}

void Entity::draw(sf::RenderWindow& window)
{
    sprite.syncPos();

    sprite.draw(window);
}

sf::Vector2f Entity::getPosition() { return position.getPosition(); }

GamePosition* Entity::getPositionVar() { return &position; }

Sprite* Entity::getSprite() { return &sprite; }