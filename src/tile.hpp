#pragma once

#include "common.hpp"

class Tile
{
public:
    Tile();

    Tile(sf::Vector2f position, sf::Vector2f size, int type, bool collides = false, std::string colliderName = "none", sf::Vector2f collOffsetFraction = {0.f, 0.f}, sf::Vector2f collSizeFraction = {1.f, 1.f});

    void init(sf::Vector2f position, sf::Vector2f size, int type, bool collides = false, std::string colliderName = "none", sf::Vector2f collOffsetFraction = {0.f, 0.f}, sf::Vector2f collSizeFraction = {1.f, 1.f});

    sf::Vector2f getPosition();
    
    sf::Vector2f getSize();

    int getType();

    bool hasCollider();

    sf::FloatRect getRect();

    std::string getColliderName();
private:
    sf::Vector2f position;

    sf::Vector2f size;

    int type;

    bool collides;

    sf::Vector2f collOffsetFraction;

    sf::Vector2f collSizeFraction;

    std::string colliderName;
};