#pragma once

#include "common.hpp"
#include "camera.hpp"
#include "asset_manager.hpp"
#include "ui_element.hpp"
#include "ui_background.hpp"

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

    AssetManager* assetManager;

    Camera* camera;

    sf::View UIView;

    std::vector<UIBackground> bgElements;

    std::vector<sf::Vertex> bgVertices;
};