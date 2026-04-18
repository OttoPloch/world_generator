#include "entity.hpp"
#include "../core/game.hpp"
#include "../world/world_position.hpp"
#include "components/collision_component.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

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

void Entity::draw(sf::RenderWindow& window)
{
    sprite.syncPos();

    sprite.draw(window);
}

sf::Vector2f Entity::getPosition() { return position.getPos(); }

WorldPosition* Entity::getPositionVar() { return &position; }

Sprite* Entity::getSprite() { return &sprite; }