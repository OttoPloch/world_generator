#pragma once

#include <SFML/Graphics.hpp>

class Game;
class Input;
class UIElement;
class UIComponent;

class Cursor
{
public:
    Cursor();

    // alternativeKeyFor____Click parameters are optional alternatives for clicking ui while in UIMode. If left blank,
    // the only way to click ui on Keyboard and Mouse when in UIMode will be by pressing the mouse buttons (even though
    // the cursor is not visible, so you are clicking with the mouse while moving the selector with the movement keys).
    Cursor(Game* game, std::string alternativeKeyForLeftClick = "NONE", std::string alternativeKeyForRightClick = "NONE");

    void inputUpdate(float dt);

    void mouseMoveEvent(sf::Event::MouseMoved mouseMoved);
    
    bool isUIModeActive();
    bool isUsingMovementForUISelector();

    // gets the coordinates of the cursor in the window,
    // not accounting for the current view.
    sf::Vector2f getGameCursorPosition();
    // gets the coordinates of the cursor in the current view.
    // So, if the camera is offset, the cursor position will inherit that offset.
    sf::Vector2f getGameCursorCoords();
    
    UIElement* getSelectedElement();
    UIComponent* getSelectedComponent();

    bool processMouseButtonEvent(sf::Event::MouseButtonPressed mouseButtonPressed, std::string& responseInput);

    bool canDoInputType(std::string type);

    bool getMouseClick(sf::Mouse::Button mouseButton);
private:
    void moveUISelector(sf::Vector2f direction);
    
    Game* game;
    Input* input;
    
    sf::Vector2f gameCursorPosition;
    UIElement* cursorElement;

    bool UIMode;
    sf::Clock UIMoveClock;
    bool usingMovementForUISelector; // only used so that Input::getMovement() can be called with UIMode on and not return (0, 0)
    bool mouseMovedThisFrame; // used to decide when to switch between controller and mouse cursor movement;

    UIElement* UISelector;
    UIElement* selectedElement;
    UIComponent* selectedComponent;

    std::string alternativeKeyForLeftClick;
    std::string alternativeKeyForRightClick;
};