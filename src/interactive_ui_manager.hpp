#pragma once

#include "common.hpp"
#include "ui_button.hpp"
#include "ui_background.hpp"

#include <map>

class Game;

class InteractiveUIManager
{
public:
    InteractiveUIManager();
    
    InteractiveUIManager(Game* game, std::map<int, std::unique_ptr<UIButton>>* buttons, UIBackground controllerUI_indicator);

    void init(Game* game, std::map<int, std::unique_ptr<UIButton>>* buttons, UIBackground controllerUI_indicator);

    void moveIndicator(sf::Vector2i direction);

    void click();
    
    void disableControllerUI();

    bool isControllerUIActive();

    int getSelectedElementID();

    void reset();

    void update();
    
    void draw();

    UIBackground controllerUI_indicator;

    int controllerUI_selectedElement;
private:
    Game* game;

    UILayer* uiLayer;

    std::map<int, std::unique_ptr<UIButton>>* buttons;

    sf::Vector2f indicatorSize;
};