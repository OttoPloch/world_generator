#pragma once

#include <memory>

#include "common.hpp"

class Sprite
{
public:
    Sprite();

    void create(std::string path, sf::Vector2f position, sf::Vector2f size, bool centerOrigin = true);

    void setPosition(sf::Vector2f newPosition);

    void setSize(sf::Vector2f newSize);

    void setRotation(float newRotation);

    void draw(sf::RenderWindow& window);
    
    sf::Sprite getSprite();
private:
    sf::Vector2f position;

    sf::Vector2f size;

    float rotation;

    sf::Texture texture;

    std::unique_ptr<sf::Sprite> sprite;
};