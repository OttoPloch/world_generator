#pragma once

#include "ui_element.hpp"
#include "tile_set.hpp"

#include <vector>

class Game;

class UIBackground : public UIElement
{
public:
    UIBackground();

    UIBackground(Game* game, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, sf::Color color, TileSet* tileSet, sf::Texture* borderTexture, std::string parentName = "");

    void init(sf::Color color, TileSet* tileSet, sf::Texture* borderTexture);

    void createBorderVertices();

    sf::Color getColor();

    UIBackground* getAsBackground() override;

    void resize(sf::Vector2f newSize, int posSet = -1) override;
    
    void draw() override;
private:
    sf::Color color;

    TileSet* tileSet;

    sf::RenderStates borderStates;
    
    std::vector<sf::Vertex> borderVertices;
    
    sf::RectangleShape background;
};