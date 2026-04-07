#pragma once

#include "../core/common.hpp"
#include "../core/camera.hpp"
#include "../graphics/asset_manager.hpp"
#include "ui_element.hpp"
#include "ui_background.hpp"
#include "ui_text.hpp"
#include "ui_button.hpp"
#include "interactive_ui_manager.hpp"

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

    void reset();

    void UIUpdate(float dt);

    void draw();
    
    InteractiveUIManager interactiveUIManager;
private:
    Game* game;

    AssetManager* assetManager;

    Camera* camera;

    int IDCounter;
    
    sf::View UIView;
    
    std::map<int, std::unique_ptr<UIElement>> elements;
};