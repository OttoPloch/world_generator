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