#pragma once

#include "../core/common.hpp"
#include "../utils/game_position.hpp"

class UIElement
{
public:
    UIElement(Game* game, std::string name, GamePosition position, sf::Vector2f size, int z, sf::Color backgroundColor = sf::Color(0, 0, 0, 0));

    void updateVertices();

    void draw();

    Game* game;

    std::string name;

    GamePosition position;
    sf::Vector2f size;

    int z;

    sf::Color backgroundColor;
    std::array<sf::Vertex, 6> vertices;
};