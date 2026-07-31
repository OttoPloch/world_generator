#pragma once

#include "common.hpp"
#include "cursor.hpp"

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



    // Mouse left and right click are treated like keys, and are referenced as "LEFTCLICK" and "RIGHTCLICK" respectively.
    // .
    // Putting "TYPE:UI" or "TYPE:WORLD" at the end of a key (with a space before it!) will make it go through Cursor::getCursorInput() to make
    // sure the cursor is in that part of the screen. If the cursor is not, the input will not be considered pressed.
    bool isKeyPressed(std::string key);
    // Putting "TYPE:UI" or "TYPE:WORLD" at the end of a button (with a space before it!) will make it go through Cursor::getCursorInput() to make
    // sure the cursor is in that part of the screen. If the cursor is not, the input will not be considered pressed.
    bool isButtonPressed(std::string button); // 'DPAD xxxx' works here, also 'LTRIGGER' and 'RTRIGGER'
    bool isControlPressed(std::string control);


    
    // The three get___ functions below return values stored from EventHandler, meaning if they return true the
    // input should be considered just pressed. For continuous input, use the is___Pressed functions above.



    // Enter key in all caps.
    // Mouse left and right click are included (even though they are not keys)
    // so that they are supported in controls.
    // .
    // Putting "TYPE:UI" or "TYPE:WORLD" at the end of a key (with a space before it!) will make it go through Cursor::getCursorInput() to make
    // sure the cursor is in that part of the screen. If the cursor is not, the input will not be considered pressed.
    bool getKey(std::string key);
    // Dpad buttons are considered axes, use getAxis() or isButtonPressed() for those.
    // .
    // Putting "TYPE:UI" or "TYPE:WORLD" at the end of a button (with a space before it!) will make it go through Cursor::getCursorInput() to make
    // sure the cursor is in that part of the screen. If the cursor is not, the input will not be considered pressed.
    bool getButton(std::string key);
    bool getControl(std::string control);

    float getAxis(sf::Joystick::Axis axis);
    
    sf::Vector2f getMovement();

    void inputUpdate(float dt);

    // These ___Event functions are meant to be only used by the EventHandler
    // left and right mouse buttons are put into keysPressedThisFrame (even though they are not keys).
    // This is to support those buttons for controls.
    void mouseButtonEvent(sf::Event::MouseButtonPressed mouseButtonPressed);
    void keyEvent(sf::Event::KeyPressed keyPressed);
    void buttonEvent(sf::Event::JoystickButtonPressed buttonPressed);

    void resetPressedThisFrame();

    std::unique_ptr<Cursor> cursor;
private:
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

    std::unordered_map<std::string, float> updateBlame;
    sf::Clock debugClock;
};