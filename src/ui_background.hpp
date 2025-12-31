#pragma once

#include "ui_element.hpp"
#include "ui_border.hpp"
#include "tile_set.hpp"

#include <vector>

class Game;

class UIBackground : public UIElement
{
public:
    UIBackground();

    UIBackground(Game* game, sf::Color color, TileSet* tileSet, UILayer* uiLayer, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, int parentIndex = -1);

    void init(Game* game, sf::Color color, TileSet* tileSet);

    void createBorderVertices();

    void draw();

    sf::Color getColor();
private:
    Game* game;

    sf::Color color;

    TileSet* tileSet;

    std::vector<sf::Vertex> borderVertices;

    sf::RenderStates borderStates;
};