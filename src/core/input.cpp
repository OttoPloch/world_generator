#include "input.hpp"
#include "game.hpp"
#include "../ui/ui_element.hpp"
#include "../ui/components/ui_component.hpp"
#include "../ui/components/image_component.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdint>
#include <iostream>
#include <algorithm>

Input::Input() {}

Input::Input(Game* game) : game(game)
{
    cursor = std::make_unique<Cursor>(game, "SPACE", "LCONTROL");

    keys = {
        "A",
        "B",
        "C",
        "D",
        "E",
        "F",
        "G",
        "H",
        "I",
        "J",
        "K",
        "L",
        "M",
        "N",
        "O",
        "P",
        "Q",
        "R",
        "S",
        "T",
        "U",
        "V",
        "W",
        "X",
        "Y",
        "Z",
        "NUM0",
        "NUM1",
        "NUM2",
        "NUM3",
        "NUM4",
        "NUM5",
        "NUM6",
        "NUM7",
        "NUM8",
        "NUM9",
        "ESCAPE",
        "LCONTROL",
        "LSHIFT",
        "LALT",
        "LSYSTEM",
        "RCONTROL",
        "RSHIFT",
        "RALT",
        "RSYSTEM",
        "MENU",
        "LBRACKET",
        "RBRACKET",
        "SEMICOLON",
        "COMMA",
        "PERIOD",
        "APOSTROPHE",
        "SLASH",
        "BACKSLASH",
        "GRAVE",
        "EQUAL",
        "HYPHEN",
        "SPACE",
        "ENTER",
        "BACKSPACE",
        "TAB",
        "PAGEUP",
        "PAGEDOWN",
        "END",
        "HOME",
        "INSERT",
        "DELETE",
        "ADD",
        "SUBTRACT",
        "MULTIPLY",
        "DIVIDE",
        "LEFT",
        "RIGHT",
        "UP",
        "DOWN",
        "NUMPAD0",
        "NUMPAD1",
        "NUMPAD2",
        "NUMPAD3",
        "NUMPAD4",
        "NUMPAD5",
        "NUMPAD6",
        "NUMPAD7",
        "NUMPAD8",
        "NUMPAD9",
        "F1",
        "F2",
        "F3",
        "F4",
        "F5",
        "F6",
        "F7",
        "F8",
        "F9",
        "F10",
        "F11",
        "F12",
        "F13",
        "F14",
        "F15",
        "PAUSE"
    };

    for (int i = 0; i < keys.size(); i++)
    {
        stringToKey[keys[i]] = i;
        keyToString[i] = keys[i];
    }

    buttons = {
        "A",
        "B",
        "X",
        "Y",
        "RBUMPER",
        "LBUMPER",
        "SELECT",
        "START",
        "?",
        "LSTICK",
        "RSTICK",
        "LTRIGGER",
        "RTRIGGER"
    };

    // ALSO DPAD LEFT,
    //      DPAD RIGHT,
    //      DPAD UP,
    //      DPAD DOWN,
    //      LTRIGGER,
    //      RTRIGGER
    // BUT THEY REGISTER AS AXES

    for (int i = 0; i < buttons.size(); i++)
    {
        stringToButton[buttons[i]] = i;
        buttonToString[i] = buttons[i];
    }

    // mouse left and right click are treated like keys, and are referenced as "LEFTCLICK" and "RIGHTCLICK" respectively.
    // putting "TYPE:UI" or "TYPE:WORLD" at the end of a key or button (with a space before it!) will make it go through Cursor::getCursorInput() to make
    // sure the cursor is in that part of the screen. If the cursor is not, the input will not be considered pressed.
    controls = {
        {"SPRINT", {"LSHIFT", "X"}},
        {"INTERACT", {"E TYPE:WORLD", "A TYPE:WORLD"}},
        {"MENU", {"Q", "B"}},
        {"EXIT", {"ESCAPE", "Y"}},
        {"MAIN ACTION", {"LEFTCLICK TYPE:WORLD", "RTRIGGER TYPE:WORLD"}},
        {"SECONDARY ACTION", {"RIGHTCLICK TYPE:WORLD", "LTRIGGER TYPE:WORLD"}},
        {"UI PRESS", {"LEFTCLICK TYPE:UI", "A TYPE:UI"}},
        {"ZOOMIN", {"NONE", "RBUMPER"}},
        {"ZOOMOUT", {"NONE", "LBUMPER"}},
        {"RESETZOOM", {"ENTER", "SELECT"}},
        {"PAUSE", {"TAB", "START"}},
        {"STEP", {"RIGHT", "RSTICK"}},
        {"TOGGLEFOCUS", {"F1", "LSTICK"}},
        {"DEBUG_VIEW", {"F2", "NONE"}},
        {"EXTRA 1", {"P", "NONE"}},
        {"EXTRA 2", {"O", "NONE"}}
    };
}

bool Input::isKeyPressed(std::string key)
{
    if (!game->getWindow()->getWindow().hasFocus()) return false;
    if (key == "NONE") return false;

    auto typeIndex = key.find("TYPE:");
    if (typeIndex != std::string::npos)
    {
        if (!cursor->canDoInputType(key.substr(typeIndex))) return false;

        key = key.substr(0, typeIndex - 1);
    }

    if (stringToKey.find(key) == stringToKey.end())
    {
        if (key.find("LEFTCLICK") == std::string::npos && key.find("RIGHTCLICK") == std::string::npos)
        {
            std::cout << "ERROR in Input::isKeyPressed(). Trying to get key '" << key << "'. That's not a key!\n";
            return false;
        }
    }
    
    if (key.find("LEFTCLICK") != std::string::npos) return cursor->getMouseClick(sf::Mouse::Button::Left);
    else if (key.find("RIGHTCLICK") != std::string::npos) return cursor->getMouseClick(sf::Mouse::Button::Right);
    else return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(stringToKey[key]));
}

bool Input::isButtonPressed(std::string button)
{
    if (!game->getWindow()->getWindow().hasFocus()) return false;
    if (!sf::Joystick::isConnected(0)) return false;
    if (button == "NONE") return false;
    
    auto typeIndex = button.find("TYPE:");
    if (typeIndex != std::string::npos)
    {
        if (!cursor->canDoInputType(button.substr(typeIndex))) return false;

        button = button.substr(0, typeIndex - 1);
    }

    if (stringToButton.find(button) == stringToButton.end())
    {
        std::cout << "ERROR in Input::isButtonPressed(). Trying to get button '" << button << "'. That's not a button!\n";
        return false;
    }

    bool pressed = false;

    if (button.substr(0, 4) == "DPAD")
    {
        if (button == "DPAD LEFT")
        {
            pressed = getAxis(sf::Joystick::Axis::PovX) < 0;
        }
        else if (button == "DPAD RIGHT")
        {
            pressed = getAxis(sf::Joystick::Axis::PovX) > 0;
        }
        else if (button == "DPAD UP")
        {
            pressed = getAxis(sf::Joystick::Axis::PovY) < 0;
        }
        else if (button == "DPAD DOWN")
        {
            pressed = getAxis(sf::Joystick::Axis::PovY) > 0;
        }
        else
        {
            std::cout << "'" << button << "' is not a button.\n";
            return false;
        }
    }
    else if (button.substr(1) == "TRIGGER")
    {
        if (button == "LTRIGGER")
        {
            pressed = getAxis(sf::Joystick::Axis::Z) > game->getSettings()->input_triggerMinPressValue;
        }
        else if (button == "RTRIGGER")
        {
            pressed = getAxis(sf::Joystick::Axis::R) > game->getSettings()->input_triggerMinPressValue;
        }
        else
        {
            std::cout << "'" << button << "' is not a button.\n";
            return false;
        }
    }
    else
    {
        pressed = sf::Joystick::isButtonPressed(0, stringToButton[button]);
    }

    return pressed;
}

bool Input::isControlPressed(std::string control)
{
    if (!game->getWindow()->getWindow().hasFocus()) return false;
    if (control == "NONE") return false;

    if (controls.find(control) == controls.end())
    {
        std::cout << "ERROR in Input::isControlPressed(). Trying to get control '" << control << "'. That's not a control!\n";
        return false;
    }
    
    return (isKeyPressed(controls[control].first) || isButtonPressed(controls[control].second));
}

bool Input::getKey(std::string key)
{
    if (key == "NONE") return false;

    auto typeIndex = key.find("TYPE:");
    if (typeIndex != std::string::npos)
    {
        if (!cursor->canDoInputType(key.substr(typeIndex))) return false;

        key = key.substr(0, typeIndex - 1);
    }

    if (key.find("LEFTCLICK") != std::string::npos) return cursor->getMouseClick(sf::Mouse::Button::Left);
    else if (key.find("RIGHTCLICK") != std::string::npos) return cursor->getMouseClick(sf::Mouse::Button::Right);
    else return keysPressedThisFrame[key];
}

bool Input::getButton(std::string button)
{
    if (button == "NONE") return false;

    auto typeIndex = button.find("TYPE:");
    if (typeIndex != std::string::npos)
    {
        if (!cursor->canDoInputType(button.substr(typeIndex))) return false;

        button = button.substr(0, typeIndex - 1);
    }

    return buttonsPressedThisFrame[button];
}

bool Input::getControl(std::string control)
{
    if (control == "NONE") return false;
    return controlsPressedThisFrame[control];
}

float Input::getAxis(sf::Joystick::Axis axis)
{
    if (sf::Joystick::isConnected(0))
    {
        return sf::Joystick::getAxisPosition(0, axis);
    }

    return 0.f;
}

sf::Vector2f Input::getMovement()
{
    if (game->getWindow()->getWindow().hasFocus() && (!cursor->isUIModeActive() || cursor->isUsingMovementForUISelector()))
    {
        sf::Vector2i movement = {0, 0};

        if (isKeyPressed("W")) movement.y -= 1;
        if (isKeyPressed("A")) movement.x -= 1;
        if (isKeyPressed("S")) movement.y += 1;
        if (isKeyPressed("D")) movement.x += 1;
        
        if (movement == sf::Vector2i(0, 0) && sf::Joystick::isConnected(0))
        {
            sf::Vector2i joystickMovement;

            joystickMovement.x = std::round(getAxis(sf::Joystick::Axis::X) / 100.f);
            joystickMovement.y = std::round(getAxis(sf::Joystick::Axis::Y) / 100.f);
            
            if (joystickMovement.x != 0 && joystickMovement.y != 0)
            {
                return {toFloat(joystickMovement.x) / toFloat(sqrt(2.f)), toFloat(joystickMovement.y) / toFloat(sqrt(2.f))};
            }
            else
            {
                return toV2F(joystickMovement.x, joystickMovement.y);
            }
        }

        if (movement.x != 0 && movement.y != 0)
        {
            return {toFloat(movement.x) / toFloat(sqrt(2.f)), toFloat(movement.y) / toFloat(sqrt(2.f))};
        }
        else
        {
            return toV2F(movement.x, movement.y);
        }
    }
    
    return {0.f, 0.f};
}

void Input::inputUpdate(float dt)
{
    updateBlame.clear();
    debugClock.restart();

    updateBlame["GAME CURSOR"] = debugClock.restart().asSeconds();
    
    if (game->getWindow()->getWindow().hasFocus())
    {
        for (auto c : controls)
        {
            if (getKey(c.second.first) || getButton(c.second.second))
            {
                controlsPressedThisFrame[c.first] = true;

                game->processInput(c.first);
            }
        }
    }

    cursor->inputUpdate(dt);

    updateBlame["CONTROLS/PROCESSING"] = debugClock.restart().asSeconds();

    if (game->getScene()->debugMode && game->getScene()->debugLevel == 1) printBlameStats(updateBlame, "INPUT_UPDATE");
}

void Input::mouseButtonEvent(sf::Event::MouseButtonPressed mouseButtonPressed)
{
    std::string responseInput;

    if (cursor->processMouseButtonEvent(mouseButtonPressed, responseInput))
    {
        keysPressedThisFrame[responseInput] = true;
    }
}
    
void Input::keyEvent(sf::Event::KeyPressed keyPressed)
{
    keysPressedThisFrame[keyToString[static_cast<int>(keyPressed.code)]] = true;
}

void Input::buttonEvent(sf::Event::JoystickButtonPressed buttonPressed)
{
    buttonsPressedThisFrame[buttonToString[static_cast<int>(buttonPressed.button)]] = true;
}

void Input::resetPressedThisFrame()
{
    keysPressedThisFrame.clear();
    buttonsPressedThisFrame.clear();
    controlsPressedThisFrame.clear();
}