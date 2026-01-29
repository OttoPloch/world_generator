#pragma once

#include "utils.hpp"
#include "ui_element.hpp"

#include <SFML/Graphics.hpp>

class UILayer;
class Game;

class UIAnimation
{
public:
    UIAnimation();

    UIAnimation(sf::Vector2f startCenter, sf::Vector2f endCenter, int endPosSet);
    
    void init(sf::Vector2f startCenter, sf::Vector2f endCenter, int endPosSet);

    sf::Vector2f step(float dt, Game* game);

    bool isDone();

    void finalize(UILayer* uiLayer, UIElement* element);

    sf::Vector2f currentCenter;
    
    sf::Vector2f startCenter;
private:
    Game* game;

    sf::Vector2f endCenter;

    int endPosSet;
};