#pragma once

#include "../core/common.hpp"
#include "../core/camera.hpp"
#include "../graphics/asset_manager.hpp"
#include "ui_element.hpp"

#include <vector>
#include <map>

class Game;

class UILayer
{
public:
    UILayer();

    void init(Game* game, Camera* camera);
    
    UIElement* getElement(std::string name);

    int getNewID();

    sf::Vector2u getScreenSize();

    sf::Vector2f getViewSize();

    bool checkUICollision();

    void updateVisuals();

    void UIUpdate(float dt);

    void draw(bool debug);
    
    // InteractiveUIManager interactiveUIManager;
private:
    Game* game;
    AssetManager* assetManager;
    Camera* camera;

    int IDCounter;
    
    sf::View UIView;
    
    std::vector<std::unique_ptr<UIElement>> elements;

    std::vector<sf::Vertex> debugWorldElementBoundingBoxes;
    std::vector<sf::Vertex> debugScreenElementBoundingBoxes;
};