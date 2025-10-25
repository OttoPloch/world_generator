#pragma once

#include <SFML/Graphics.hpp>

#include "common.hpp"
#include "window.hpp"

class EventHandler
{
public:
    EventHandler();

    void init(Window& window);

    void processEvents();

    void windowClosed();

    void keyPressed(sf::Keyboard::Key key);
private:
    Window* window;
};