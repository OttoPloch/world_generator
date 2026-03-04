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

    void spriteInit(sf::Texture* texture, sf::Vector2f size = {1.f, 1.f}, bool sizeIsScale = true);

    int getID();

    void tick();

    void update(float dt);

    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition();

    Sprite* getSprite();
private:
    Game* game;

    int ID;

    WorldPosition position;

    Sprite sprite;
};