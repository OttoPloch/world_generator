#include "input.hpp"

bool getKey(sf::Keyboard::Key key)
{
    if (sf::Keyboard::isKeyPressed(key))
    {
        return true;
    }

    return false;
}
    
bool getKey(std::string key)
{
    if (key == "A" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) return true;
    if (key == "B" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B)) return true;
    if (key == "C" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) return true;
    if (key == "D" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) return true;
    if (key == "E" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) return true;
    if (key == "F" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) return true;
    if (key == "G" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)) return true;
    if (key == "H" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)) return true;
    if (key == "I" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) return true;
    if (key == "J" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J)) return true;
    if (key == "K" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)) return true;
    if (key == "L" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) return true;
    if (key == "M" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M)) return true;
    if (key == "N" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N)) return true;
    if (key == "O" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O)) return true;
    if (key == "P" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) return true;
    if (key == "Q" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) return true;
    if (key == "R" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) return true;
    if (key == "S" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) return true;
    if (key == "T" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) return true;
    if (key == "U" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U)) return true;
    if (key == "V" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V)) return true;
    if (key == "W" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) return true;
    if (key == "X" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) return true;
    if (key == "Y" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y)) return true;
    if (key == "Z" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) return true;
    if (key == "LEFT" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) return true;
    if (key == "RIGHT" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) return true;
    if (key == "UP" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) return true;
    if (key == "DOWN" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) return true;
    if (key == "ESCAPE" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) return true;
    if (key == "TAB" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab)) return true;
    if (key == "SHIFT" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) return true;
    if (key == "CONTROL" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) return true;
    if (key == "ENTER" && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) return true;

    return false;
}

bool getButton(std::string key)
{
    if (sf::Joystick::isConnected(0))
    {
        if (key == "A" && sf::Joystick::isButtonPressed(0, 0)) return true;
        if (key == "B" && sf::Joystick::isButtonPressed(0, 1)) return true;
        if (key == "X" && sf::Joystick::isButtonPressed(0, 2)) return true;
        if (key == "Y" && sf::Joystick::isButtonPressed(0, 3)) return true;
    }

    return false;
}

bool getControl(std::string key)
{
    if (key == "SPRINT" && (getKey("SHIFT") || getButton("A"))) return true;
    if (key == "INTERACT" && (getKey("E") || getButton("X"))) return true;
    if (key == "BACK" && (getKey("Q") || getButton("B"))) return true;

    return false;
}

sf::Vector2f getMovement()
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