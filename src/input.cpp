#include "input.hpp"
#include "game.hpp"
#include "interactive_ui_manager.hpp"

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

    // ALSO DPAD LEFT,
    //      DPAD RIGHT,
    //      DPAD UP,
    //      DPAD DOWN
    // BUT THEY REGISTER AS AXES

    for (int i = 0; i < buttons.size(); i++)
    {
        stringToButton[buttons[i]] = i;

        buttonToString[i] = buttons[i];
    }

    controls = {
        {"SPRINT", {"LSHIFT", "X"}},
        {"INTERACT", {"E", "A"}},
        {"MENU", {"Q", "B"}},
        {"EXIT", {"ESCAPE", "Y"}},
        {"PAUSE", {"TAB", "START"}},
        {"STEP", {"RIGHT", "RSTICK"}},
        {"RESETZOOM", {"ENTER", "SELECT"}},
        {"TOGGLEFOCUS", {"F1", "LSTICK"}},
        {"ZOOMIN", {"NONE", "LBUMPER"}},
        {"ZOOMOUT", {"NONE", "RBUMPER"}},
        {"UI LEFT", {"LEFT", "DPAD LEFT"}},
        {"UI RIGHT", {"RIGHT", "DPAD RIGHT"}},
        {"UI UP", {"UP", "DPAD UP"}},
        {"UI DOWN", {"DOWN", "DPAD DOWN"}}
    };

    leftClickThisFrame = false;
    leftClickLastFrame = false;
}

bool Input::getKey(std::string key)
{
    if (game->getWindow()->getWindow().hasFocus())
    {
        bool isPressed = false;
    
        if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(stringToKey[key]))) isPressed = true;
    
        if (isPressed) keysPressedThisFrame[key] = true;
    
        return isPressed;
    }
    
    return false;
}

bool Input::getButton(std::string key)
{
    if (game->getWindow()->getWindow().hasFocus())
    {
        bool isPressed = false;

        if (sf::Joystick::isConnected(0))
        {
            if (key.substr(0, 4) == "DPAD")
            {
                if (key == "DPAD LEFT")
                {
                    if (getAxis(sf::Joystick::Axis::PovX) < 0.f) isPressed = true;
                }
                else if (key == "DPAD RIGHT")
                {
                    if (getAxis(sf::Joystick::Axis::PovX) > 0.f) isPressed = true;
                }
                else if (key == "DPAD UP")
                {
                    if (getAxis(sf::Joystick::Axis::PovY) < 0.f) isPressed = true;
                }
                else if (key == "DPAD DOWN")
                {
                    if (getAxis(sf::Joystick::Axis::PovY) > 0.f) isPressed = true;
                }
                else
                {
                    std::cout << "ERROR in getButton with DPAD key, key is " << key << ". That's not a button!\n";
                }
            }
            else
            {
                if (sf::Joystick::isButtonPressed(0, toUnsignedInt(stringToButton[key]))) isPressed = true;
            }
        }

        if (isPressed) buttonsPressedThisFrame[key] = true;

        return isPressed;
    }

    return false;
}

bool Input::getControl(std::string key)
{
    if (game->getWindow()->getWindow().hasFocus())
    {
        bool isPressed = false;

        for (int i = 0; i < controls.size(); i++)
        {
            if (controls[i].first == key)
            {
                if ((controls[i].second.first != "NONE" && getKey(controls[i].second.first)) || (controls[i].second.second != "NONE" && getButton(controls[i].second.second)))
                {
                    isPressed = true;
                }
            }
        }

        if (isPressed) controlsPressedThisFrame[key] = true;

        return isPressed;
    }
    
    return false;
}

bool Input::leftClick()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !leftClickLastFrame)
    {
        leftClickThisFrame = true;

        return true;
    }

    return false;
}

float Input::getAxis(sf::Joystick::Axis axis)
{
    if (sf::Joystick::isConnected(0))
    {
        return sf::Joystick::getAxisPosition(0, axis);
    }

    return 0.f;
}

float Input::getAxis(int axis)
{
    if (sf::Joystick::isConnected(0))
    {
        return sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(axis));
    }

    return 0.f;
}

sf::Vector2f Input::getMovement()
{
    if (game->getWindow()->getWindow().hasFocus())
    {
        sf::Vector2i movement = {0, 0};

        if (getKey("W")) movement.y -= 1;
        if (getKey("A")) movement.x -= 1;
        if (getKey("S")) movement.y += 1;
        if (getKey("D")) movement.x += 1;
        
        if (movement == sf::Vector2i(0, 0) && sf::Joystick::isConnected(0))
        {
            sf::Vector2i joystickMovement;

            joystickMovement.x = std::round(getAxis(0) / 100.f);
            joystickMovement.y = std::round(getAxis(1) / 100.f);
            
            if (joystickMovement.x != 0 && joystickMovement.y != 0)
            {
                return {toFloat(joystickMovement.x) / toFloat(sqrt(2.f)), toFloat(joystickMovement.y) / toFloat(sqrt(2.f))};
            }

            return toV2F(joystickMovement.x, joystickMovement.y);
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
    else
    {
        return {0.f, 0.f};
    }
}

void Input::update()
{
    if (game->getWindow()->getWindow().hasFocus())
    {
        for (int i = 0; i < controls.size(); i++)
        {
            if (getControl(controls[i].first) && !controlsPressedLastFrame[controls[i].first])
            {
                game->processInput(controls[i].first);
            }
        }

        leftClickLastFrame = leftClickThisFrame;
        leftClickThisFrame = false;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            leftClickThisFrame = true;

            game->getScene()->getUILayer()->interactiveUIManager.disableControllerUI();
        }
    
        if (controllerUI_moveClock.getElapsedTime().asSeconds() >= 0.2f)
        {
            sf::Vector2i UIMoveInput;
        
            UIMoveInput.x = std::round(getAxis(4) / 100.f);
            UIMoveInput.y = std::round(getAxis(5) / 100.f);
            
            sf::Vector2i moveDirection = {0, 0};
            
            if (getControl("UI LEFT") || getControl("UI RIGHT") || getControl("UI UP") || getControl("UI DOWN"))
            {
                if (getControl("UI LEFT")) moveDirection.x = -1;
                if (getControl("UI RIGHT")) moveDirection.x = 1;
                if (getControl("UI UP")) moveDirection.y = -1;
                if (getControl("UI DOWN")) moveDirection.y = 1;
            }
            else
            {
                if (UIMoveInput.x != 0 || UIMoveInput.y != 0)
                {
                    (abs(getAxis(4)) > abs(getAxis(5))) ? moveDirection.x = UIMoveInput.x : moveDirection.y = UIMoveInput.y;
                }
            }

            if (UIMoveInput.x != 0 || UIMoveInput.y != 0 || moveDirection.x != 0 || moveDirection.y != 0)
            {
                game->getScene()->getUILayer()->interactiveUIManager.moveIndicator(moveDirection);
                
                controllerUI_moveClock.restart();
            }
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
