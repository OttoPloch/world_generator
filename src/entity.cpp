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

int Entity::getID() { return ID; }

void Entity::tick()
{

}

void Entity::update(float dt)
{

}

void Entity::draw(sf::RenderWindow& window)
{
    sprite.draw(window);
}

sf::Vector2f Entity::getPosition() { return position.getPos(); }

Sprite* Entity::getSprite() { return &sprite; }