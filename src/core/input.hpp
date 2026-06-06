#pragma once

#include "common.hpp"

#include <map>
#include <vector>

class Game;

class Input
{
public:
    Input();

    void init(Game* game);

    // enter key in all caps
    bool isKeyPressed(std::string key, bool checkJustPressed = false);
    
    // enter key in all caps.
    // 'DPAD xxx' works but is
    // special because they are axes.
    // could also use getAxis for those.
    bool isButtonPressed(std::string button, bool checkJustPressed = false);
    
    bool isControlPressed(std::string control, bool checkJustPressed = false);

    float getAxis(sf::Joystick::Axis axis);
    
    float getAxis(int axis);

    sf::Vector2f getMovement();

    // gets the coordinates of the mouse in the current view.
    // So, if the camera is offset, the mouse will inherit that offset.
    sf::Vector2f getMouseCoords();

    // gets the coordinates of the mouse in the window, not
    // accounting for the current view.
    sf::Vector2f getMouseWindowPos();

    bool getKeyPressedLastFrame(std::string key);
    
    bool getControlPressedLastFrame(std::string control);

    void update();

    void shiftPressedThisFrame();
private:
    // enter key in all caps
    // Dont worry about dontSetPressedThisFrame,
    // it is only used in other functions in Input.
    bool getKey(std::string key, bool dontSetPressedThisFrame = false);
    
    // enter key in all caps.
    // 'DPAD xxx' works but is
    // special because they are axes.
    // Can also use getAxis for those.
    // Dont worry about dontSetPressedThisFrame,
    // it is only used in other functions in Input.
    bool getButton(std::string key, bool dontSetPressedThisFrame = false);
    
    // Dont worry about dontSetPressedThisFrame,
    // it is only used in other functions in Input.
    bool getControl(std::string control, bool dontSetPressedThisFrame = false);

    Game* game;

    std::vector<std::string> keys;

    std::unordered_map<std::string, int> stringToKey;

    std::unordered_map<int, std::string> keyToString;

    std::vector<std::string> buttons;

    std::unordered_map<std::string, int> stringToButton;

    std::unordered_map<int, std::string> buttonToString;

    std::vector<std::pair<std::string, std::pair<std::string, std::string>>> controls;

    std::unordered_map<std::string, bool> keysPressedThisFrame;
    std::unordered_map<std::string, bool> keysPressedLastFrame;

    std::unordered_map<std::string, bool> buttonsPressedThisFrame;
    std::unordered_map<std::string, bool> buttonsPressedLastFrame;
    
    std::unordered_map<std::string, bool> controlsPressedThisFrame;
    std::unordered_map<std::string, bool> controlsPressedLastFrame;

    sf::Clock controllerUI_moveClock;


    std::unordered_map<std::string, float> updateBlame;
    sf::Clock debugClock;
};