#pragma once

#include <SFML/Graphics.hpp>

class Game;
class Input;
class UIElement;

class Cursor
{
public:
    Cursor();

    Cursor(Game* game);

    void inputUpdate();

    void draw();

    sf::Vector2f gameCursorPosition;
private:
    Game* game;

    Input* input;

    UIElement* cursorElement;
};