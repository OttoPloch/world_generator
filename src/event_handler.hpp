#pragma once

#include "common.hpp"
#include "window.hpp"
#include "camera.hpp"

class EventHandler
{
public:
    EventHandler();

    void init(Window& window, Camera& camera);

    void processEvents();

    void windowClosed();

    void windowResized();
    
    void keyPressed(sf::Keyboard::Key key);

    void mouseWheelScrolled(const sf::Event::MouseWheelScrolled* scroll);
private:
    Window* window;

    Camera* camera;
};