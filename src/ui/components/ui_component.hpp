#pragma once

#include "../ui_position.hpp"

class Game;
class UIElement;

struct UIComponent
{
    UIComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier);

    virtual sf::FloatRect getLocalBounds();

    virtual void updateVisuals();

    virtual void draw();

    Game* game;
    UIElement* myElement;

    UIPosition position;

    std::string identifier;
};