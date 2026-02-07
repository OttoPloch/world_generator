#pragma once

#include "common.hpp"

class Game;

class Tile
{
public:
    Tile();

    Tile(sf::Vector2f position, sf::Vector2f size, int type);

    void init(sf::Vector2f position, sf::Vector2f size, int type);

    sf::Vector2f getPosition();
    
    sf::Vector2f getSize();

    int getType();
private:
    sf::Vector2f position;

    sf::Vector2f size;

    int type;
};