#pragma once

#include "../core/common.hpp"

class UIElement
{
public:
    UIElement(Game* game, std::string name, sf::Vector2f position, sf::Vector2f size, int z, sf::Color backgroundColor = sf::Color(0, 0, 0, 0));

    void draw();

    Game* game;

    std::string name;

    sf::Vector2f position;
    sf::Vector2f size;

    int z;

    std::array<sf::Vertex, 6> vertices;
};