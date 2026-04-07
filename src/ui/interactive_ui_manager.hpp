#pragma once

#include "../core/common.hpp"
#include "ui_button.hpp"
#include "ui_background.hpp"

#include <map>

class Game;

class InteractiveUIManager
{
public:
    InteractiveUIManager();
    
    InteractiveUIManager(Game* game, std::map<int, std::unique_ptr<UIElement>>* elements, UIElement* controllerUI_indicator);

    void init(Game* game, std::map<int, std::unique_ptr<UIElement>>* elements, UIElement* controllerUI_indicator);

    void moveIndicator(sf::Vector2i direction);

    void click();
    
    void disableControllerUI();

    bool isControllerUIActive();

    UIElement* getSelectedElement();

    void draw();

    void updateIndicatorPosition();

    UIElement* controllerUI_indicator;

    UIElement* controllerUI_selectedElement;
private:
    Game* game;

    UILayer* uiLayer;

    std::map<int, std::unique_ptr<UIElement>>* elements;

    bool active;

    sf::Vector2f indicatorSize;
};