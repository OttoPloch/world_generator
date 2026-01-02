#include "input.hpp"
#include "game.hpp"

Input::Input() {}

void Input::init(Game* game)
{
    this->game = game;

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
        "LBUMPER",
        "RBUMPER",
        "SELECT",
        "START",
        "?",
        "LSTICK",
        "RSTICK",
    };

    for (int i = 0; i < buttons.size(); i++)
    {
        stringToButton[buttons[i]] = i;

        buttonToString[i] = buttons[i];
    }

    controls = {
        {"SPRINT", {"LSHIFT", "A"}},
        {"INTERACT", {"E", "X"}},
        {"MENU", {"Q", "B"}},
        {"EXIT", {"ESCAPE", "Y"}},
        {"PAUSE", {"TAB", "START"}},
        {"STEP", {"RIGHT", "RSTICK"}},
        {"RESETZOOM", {"ENTER", "SELECT"}},
        {"TOGGLEFOCUS", {"F1", "LSTICK"}},
        {"ZOOMIN", {"NONE", "LBUMPER"}},
        {"ZOOMOUT", {"NONE", "RBUMPER"}}
    };
}

bool Input::getKey(std::string key)
{
    bool isPressed = false;

    if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(stringToKey[key]))) isPressed = true;

    if (isPressed) keysPressedThisFrame[key] = true;

    return isPressed;
}

bool Input::getButton(std::string key)
{
    bool isPressed = false;

    if (sf::Joystick::isConnected(0))
    {
        if (sf::Joystick::isButtonPressed(0, toUnsignedInt(stringToButton[key]))) isPressed = true;
    }

    if (isPressed) buttonsPressedThisFrame[key] = true;

    return isPressed;
}

bool Input::getControl(std::string key)
{
    bool isPressed = false;

    for (int i = 0; i < controls.size(); i++)
    {
        if (controls[i].first == key)
        {
            if (getKey(controls[i].second.first) || getButton(controls[i].second.second)) isPressed = true;
        }
    }

    if (isPressed) controlsPressedThisFrame[key] = true;

    return isPressed;
}

sf::Vector2f Input::getMovement()
{
    sf::Vector2i movement = {0, 0};

    if (getKey("W")) movement.y -= 1;
    if (getKey("A")) movement.x -= 1;
    if (getKey("S")) movement.y += 1;
    if (getKey("D")) movement.x += 1;
    
    if (movement == sf::Vector2i(0, 0) && sf::Joystick::isConnected(0))
    {
        sf::Vector2f joystickMovement;

        // TODO: fix the joystick input so that the two values add up to 1, not over.
        joystickMovement.x = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) / 100.f;
        joystickMovement.y = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) / 100.f;

        return joystickMovement;
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

void Input::update()
{
    for (int i = 0; i < controls.size(); i++)
    {
        if (getControl(controls[i].first) && !controlsPressedLastFrame[controls[i].first])
        {
            game->processInput(controls[i].first);
        }
    }

    for (auto i : keysPressedLastFrame)
    {
        keysPressedLastFrame[i.first] = false;
    }

    for (auto i : keysPressedThisFrame)
    {
        keysPressedLastFrame[i.first] = i.second;

        keysPressedThisFrame[i.first] = false;
    }

    for (auto i : buttonsPressedLastFrame)
    {
        buttonsPressedLastFrame[i.first] = false;
    }

    for (auto i : buttonsPressedThisFrame)
    {
        buttonsPressedLastFrame[i.first] = i.second;

        buttonsPressedThisFrame[i.first] = false;
    }

    for (auto i : controlsPressedLastFrame)
    {
        controlsPressedLastFrame[i.first] = false;
    }

    for (auto i : controlsPressedThisFrame)
    {
        controlsPressedLastFrame[i.first] = i.second;

        controlsPressedThisFrame[i.first] = false;
    }
}