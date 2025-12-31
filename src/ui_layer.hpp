#pragma once

#include "common.hpp"
#include "ui_element.hpp"
#include "camera.hpp"

#include <vector>

class Game;

class UILayer
{
public:
    UILayer();

    void init(Game* game, Camera* camera);

    UIElement* getElement(int index);
    
    sf::Vector2u getScreenSize();

    void resetView();
    
    void draw();
private:
    Game* game;

    Camera* camera;

    sf::View UIView;

    std::vector<UIElement> elements;

    std::vector<sf::Vertex> bgVertices;
};