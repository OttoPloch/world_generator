#include "input.hpp"
#include "game.hpp"
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

Input::Input() {}

Input::Input(Game* game) : game(game)
{
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
        {"DEBUG_VIEW", {"F2", "NONE"}},
        {"ZOOMIN", {"NONE", "LBUMPER"}},
        {"ZOOMOUT", {"NONE", "RBUMPER"}},
        {"UI LEFT", {"LEFT", "DPAD LEFT"}},
        {"UI RIGHT", {"RIGHT", "DPAD RIGHT"}},
        {"UI UP", {"UP", "DPAD UP"}},
        {"UI DOWN", {"DOWN", "DPAD DOWN"}},
        {"EXTRA 1", {"P", "NONE"}},
        {"EXTRA 2", {"O", "NONE"}},
        {"MAIN ACTION", {"LEFTCLICK", "A"}},
        {"SECONDARY ACTION", {"RIGHTCLICK", "B"}}
    };
}

bool Input::isKeyPressed(std::string key)
{
    if (key == "LEFTCLICK") return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    else if (key == "RIGHTCLICK") return sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    else
    {
        if (stringToKey.find(key) == stringToKey.end())
        {
            std::cout << "ERROR in Input::isKeyPressed(). Trying to get key '" << key << "'. That's not a key!\n";
            return false;
        }

        return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(stringToKey[key]));
    }
}

bool Input::isButtonPressed(std::string button)
{
    if (!sf::Joystick::isConnected(0)) return false;
    
    if (stringToButton.find(button) == stringToButton.end())
    {
        std::cout << "ERROR in Input::isButtonPressed(). Trying to get button '" << button << "'. That's not a button!\n";
        return false;
    }

    if (button.substr(0, 4) == "DPAD")
    {
        if (button == "DPAD LEFT")
        {
            return getAxis(sf::Joystick::Axis::PovX) < 0;
        }
        else if (button == "DPAD RIGHT")
        {
            return getAxis(sf::Joystick::Axis::PovX) > 0;
        }
        else if (button == "DPAD UP")
        {
            return getAxis(sf::Joystick::Axis::PovY) < 0;
        }
        else if (button == "DPAD DOWN")
        {
            return getAxis(sf::Joystick::Axis::PovY) > 0;
        }
        else
        {
            std::cout << "'" << button << "' is not a button.\n";
            return false;
        }
    }
    else
    {
        return sf::Joystick::isButtonPressed(0, stringToButton[button]);
    }
}

bool Input::isControlPressed(std::string control)
{
    if (controls.find(control) == controls.end())
    {
        std::cout << "ERROR in Input::isControlPressed(). Trying to get control '" << control << "'. That's not a control!\n";
        return false;
    }
    
    return (controls[control].first != "NONE" && isKeyPressed(controls[control].first)) || (controls[control].second != "NONE" && isButtonPressed(controls[control].second));
}

bool Input::getKey(std::string key)
{
    return keysPressedThisFrame[key];
}

bool Input::getButton(std::string button)
{
    return buttonsPressedThisFrame[button];
}

bool Input::getControl(std::string control)
{
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
    if (game->getWindow()->getWindow().hasFocus())
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
    else
    {
        return {0.f, 0.f};
    }
}

sf::Vector2f Input::getMouseCoords()
{
    return game->getWindow()->getWindow().mapPixelToCoords(sf::Mouse::getPosition(game->getWindow()->getWindow()));
}

sf::Vector2f Input::getMouseWindowPos()
{
    return toV2F(sf::Mouse::getPosition(game->getWindow()->getWindow()));
}

void Input::update()
{
    updateBlame.clear();
    debugClock.restart();
    
    if (game->getWindow()->getWindow().hasFocus())
    {
        for (auto c : controls)
        {
            if ((c.second.first != "NONE" && getKey(c.second.first)) || c.second.second != "NONE" && getButton(c.second.second))
            {
                controlsPressedThisFrame[c.first] = true;

                game->processInput(c.first);
            }
        }

        updateBlame["CONTROLS/PROCESSING"] = debugClock.restart().asSeconds();

        for (auto b : buttonsPressedThisFrame)
        {
            if (b.second) game->getWindow()->getWindow().setMouseCursorVisible(false);
            break;
        }

        for (auto k : keysPressedThisFrame)
        {
            if (k.second) game->getWindow()->getWindow().setMouseCursorVisible(true);
            break;
        }

        updateBlame["CURSOR VISIBILITY SET"] = debugClock.restart().asSeconds();
    }



    // TODL: OLD THINGS STILL NEEDING TO BE REIMPLEMENTED
    //   \/   \/   \/   \/   \/   \/   \/   \/  \/


    //     // if (getKey("LEFTCLICK") || getKey("RIGHTCLICK")) game->getScene()->getUILayer()->interactiveUIManager.disableControllerUI();
    
    //     if (controllerUI_moveClock.getElapsedTime().asSeconds() >= 0.2f)
    //     {
    //         sf::Vector2i UIMoveInput;
        
    //         UIMoveInput.x = std::round(getAxis(sf::Joystick::Axis::U) / 100.f);
    //         UIMoveInput.y = std::round(getAxis(sf::Joystick::Axis::V) / 100.f);
            
    //         sf::Vector2i moveDirection = {0, 0};
            
    //         if (getControl("UI LEFT") || getControl("UI RIGHT") || getControl("UI UP") || getControl("UI DOWN"))
    //         {
    //             if (getControl("UI LEFT")) moveDirection.x = -1;
    //             if (getControl("UI RIGHT")) moveDirection.x = 1;
    //             if (getControl("UI UP")) moveDirection.y = -1;
    //             if (getControl("UI DOWN")) moveDirection.y = 1;
    //         }
    //         else
    //         {
    //             if (UIMoveInput.x != 0 || UIMoveInput.y != 0)
    //             {
    //                 (std::abs(getAxis(sf::Joystick::Axis::U)) > std::abs(getAxis(sf::Joystick::Axis::V))) ? moveDirection.x = UIMoveInput.x : moveDirection.y = UIMoveInput.y;
    //             }
    //         }

    //         if (UIMoveInput.x != 0 || UIMoveInput.y != 0 || moveDirection.x != 0 || moveDirection.y != 0)
    //         {
    //             // game->getScene()->getUILayer()->interactiveUIManager.moveIndicator(moveDirection);
                
    //             controllerUI_moveClock.restart();
    //         }
    //     }

    //     updateBlame["CONTROLLER"] = debugClock.restart().asSeconds();
    // }

    if (game->getScene()->debugMode && game->getScene()->debugLevel == 1) printBlameStats(updateBlame, "INPUT_UPDATE");
}

void Input::mouseEvent(sf::Event::MouseButtonPressed mouseButtonPressed)
{
    if (mouseButtonPressed.button == sf::Mouse::Button::Left)
    {
        keysPressedThisFrame["LEFTCLICK"] = true;
    }
    else if (mouseButtonPressed.button == sf::Mouse::Button::Right)
    {
        keysPressedThisFrame["RIGHTCLICK"] = true;
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