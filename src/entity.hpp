#pragma once

#include "common.hpp"
#include "world_position.hpp"
#include "sprite.hpp"

#include <SFML/Graphics.hpp>

class Game;

class Entity
{
public:
    Entity();

    Entity(Game* game, int ID, sf::Vector2f position);

    Entity(Game* game, int ID, WorldPosition position);

    Sprite* spriteInit(sf::Texture* texture, sf::Vector2f size = {1.f, 1.f}, bool sizeIsScale = true, bool usingTexCoords = false, sf::IntRect texCoords = sf::IntRect({0, 0}, {0, 0}));

    int getID();

    virtual void tick();

    virtual void update(float dt);

    virtual void draw(float alpha, sf::RenderWindow& window);

    sf::Vector2f getPosition();

    Sprite* getSprite();
protected:
    Game* game;

    int ID;

    WorldPosition position;
    sf::Vector2f lastPosition;

    Sprite sprite;
};