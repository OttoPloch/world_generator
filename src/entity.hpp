#pragma once

#include "common.hpp"
#include "sprite.hpp"

class Entity
{
public:
    Entity();

    void create(sf::Vector2f position);

    void giveSprite(std::string path, sf::Vector2f size);

    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition();
private:
    sf::Vector2f position;

    float rotation;

    Sprite sprite;
};