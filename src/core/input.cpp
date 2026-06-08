#include "input.hpp"
#include "game.hpp"
#include "../ui/ui_element.hpp"
#include "../ui/components/image_component.hpp"
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdint>

Input::Input() {}

Input::Input(Game* game) : game(game)
{
    game->getWindow()->getWindow().setMouseCursorVisible(false);

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

    controls = {
        {"SPRINT", {"LSHIFT", "X"}},
        {"INTERACT", {"E", "A"}},
        {"MENU", {"Q", "B"}},
        {"EXIT", {"ESCAPE", "Y"}},
        {"MAIN ACTION", {"LEFTCLICK", "RTRIGGER"}},
        {"SECONDARY ACTION", {"RIGHTCLICK", "LTRIGGER"}},
        {"UI PRESS", {"LEFTCLICK", "A"}},
        {"ZOOMIN", {"NONE", "RBUMPER"}},
        {"ZOOMOUT", {"NONE", "LBUMPER"}},
        {"RESETZOOM", {"ENTER", "SELECT"}},
        {"PAUSE", {"TAB", "START"}},
        {"STEP", {"RIGHT", "RSTICK"}},
        {"UI LEFT", {"LEFT", "DPAD LEFT"}},
        {"UI RIGHT", {"RIGHT", "DPAD RIGHT"}},
        {"UI UP", {"UP", "DPAD UP"}},
        {"UI DOWN", {"DOWN", "DPAD DOWN"}},
        {"TOGGLEFOCUS", {"F1", "LSTICK"}},
        {"DEBUG_VIEW", {"F2", "NONE"}},
        {"EXTRA 1", {"P", "NONE"}},
        {"EXTRA 2", {"O", "NONE"}}
    };

    gameCursorPosition = toV2F(game->getWindow()->getSize().x / 2, game->getWindow()->getSize().y / 2);
    cursorElement = game->getScene()->getUILayer()->createElement(std::make_unique<UIElement>(game, "cursor", UIPosition(getCursorWindowPos()), INT32_MAX, nullptr));
    cursorElement->addComponent<ImageComponent>(game, cursorElement, UIPosition({0, 0}), "cursor image", 0, game->getAssetManager()->getTexture("cursor"), sf::Vector2f(30, 30), false);
    mouseMovedThisFrame = true;
}

bool Input::isKeyPressed(std::string key)
{
    if (!game->getWindow()->getWindow().hasFocus()) return false;

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
    if (!game->getWindow()->getWindow().hasFocus()) return false;
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
    else if (button.substr(1) == "TRIGGER")
    {
        if (button == "LTRIGGER")
        {
            return getAxis(sf::Joystick::Axis::Z) > game->getSettings()->input_triggerMinPressValue;
        }
        else if (button == "RTRIGGER")
        {
            return getAxis(sf::Joystick::Axis::R) > game->getSettings()->input_triggerMinPressValue;
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
    if (!game->getWindow()->getWindow().hasFocus()) return false;

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

sf::Vector2f Input::getCursorCoords()
{
    return game->getWindow()->getWindow().mapPixelToCoords(toV2I(gameCursorPosition));
}

sf::Vector2f Input::getCursorWindowPos()
{
    return gameCursorPosition;
}

void Input::inputUpdate()
{
    updateBlame.clear();
    debugClock.restart();

    if (game->getWindow()->getWindow().hasFocus())
    {
        // TEMP, need to fully implement controller support in a neat way
        if (sf::Joystick::isConnected(0) && !mouseMovedThisFrame)
        {
            // sensitivity is divided by 100 to make it simpler to set, maybe not the right thing to do.
            gameCursorPosition.x += getAxis(sf::Joystick::Axis::U) * game->getSettings()->input_controllerCursorSensitivity / 100;
            gameCursorPosition.y += getAxis(sf::Joystick::Axis::V) * game->getSettings()->input_controllerCursorSensitivity / 100;
        }
        else
        {
            gameCursorPosition = toV2F(sf::Mouse::getPosition(game->getWindow()->getWindow()));
        }
    }
    
    // limits the game cursor to on the screen.
    sf::Vector2u windowSize = game->getWindow()->getSize();
    gameCursorPosition = {std::min(std::max(gameCursorPosition.x, 0.f), toFloat(windowSize.x)), std::min(std::max(gameCursorPosition.y, 0.f), toFloat(windowSize.y))};

    cursorElement->position.position = gameCursorPosition;
    cursorElement->updateVisuals();

    updateBlame["GAME CURSOR"] = debugClock.restart().asSeconds();
    
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
    }

    // TODO: OLD THINGS STILL NEEDING TO BE REIMPLEMENTED
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
    // }

    mouseMovedThisFrame = false;

    if (game->getScene()->debugMode && game->getScene()->debugLevel == 1) printBlameStats(updateBlame, "INPUT_UPDATE");
}

void Input::mouseMoveEvent(sf::Event::MouseMoved mouseMoved)
{
    mouseMovedThisFrame = true;
}

void Input::mouseButtonEvent(sf::Event::MouseButtonPressed mouseButtonPressed)
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