#pragma once

#include "common.hpp"

#include <map>
#include <vector>

class Game;
class UIElement;
class UIComponent;

class Input
{
public:
    Input();

    Input(Game* game);

    // The three is___Pressed functions below grab the raw input values every time that function is called, meaning
    // they could be just pressed or held down. For single presses, use the get___ functions below.

    //
    bool isKeyPressed(std::string key);
    bool isButtonPressed(std::string button); // 'DPAD xxxx' works here, also 'LTRIGGER' and 'RTRIGGER'
    bool isControlPressed(std::string control);

    // The three get___ functions below return values stored from EventHandler, meaning if they return true the
    // input should be considered just pressed. For continuous input, use the is___Pressed functions above.

    // enter key in all caps
    // mouse left and right click are included (even though they are not keys)
    // so that they are supported in controls.
    bool getKey(std::string key);
    // Dpad buttons are considered axes, use getAxis() or isButtonPressed() for those.
    bool getButton(std::string key);
    bool getControl(std::string control);

    float getAxis(sf::Joystick::Axis axis);
    
    sf::Vector2f getMovement();

    // gets the coordinates of the cursor in the current view.
    // So, if the camera is offset, the cursor position will inherit that offset.
    sf::Vector2f getCursorCoords();

    // gets the coordinates of the cursor in the window, not
    // accounting for the current view.
    sf::Vector2f getCursorWindowPos();

    void inputUpdate(float dt);

    // These ___Event functions are meant to be only used by the EventHandler
    // left and right mouse buttons are put into keysPressedThisFrame (even though they are not keys).
    // this is to support those buttons for controls.
    void mouseMoveEvent(sf::Event::MouseMoved mouseMoved);
    void mouseButtonEvent(sf::Event::MouseButtonPressed mouseButtonPressed);
    void keyEvent(sf::Event::KeyPressed keyPressed);
    void buttonEvent(sf::Event::JoystickButtonPressed buttonPressed);

    void resetPressedThisFrame();

    bool isUIModeActive();
    
    UIElement* getSelectedElement();
    UIComponent* getSelectedComponent();
private:
    void moveUISelector(sf::Vector2f direction);

    Game* game;

    std::vector<std::string> keys;
    std::unordered_map<std::string, int> stringToKey;
    std::unordered_map<int, std::string> keyToString;
    std::unordered_map<std::string, bool> keysPressedThisFrame;

    std::vector<std::string> buttons;
    std::unordered_map<std::string, int> stringToButton;
    std::unordered_map<int, std::string> buttonToString;
    std::unordered_map<std::string, bool> buttonsPressedThisFrame;

    std::unordered_map<std::string, std::pair<std::string, std::string>> controls;
    std::unordered_map<std::string, bool> controlsPressedThisFrame;
    
    sf::Vector2f gameCursorPosition;
    UIElement* cursorElement;
    bool mouseMovedThisFrame;

    UIElement* UISelector;
    UIElement* selectedElement;
    UIComponent* selectedComponent;
    bool UIMode;
    bool usingMovementForUISelector; // only used so that getMovement() can be called with UIMode on and not return (0, 0)
    bool hideCursor;
    sf::Clock UIMoveClock;

    std::unordered_map<std::string, float> updateBlame;
    sf::Clock debugClock;
};