#pragma once

#include "common.hpp"
#include "window.hpp"
#include "camera.hpp"

class Game;

class EventHandler
{
public:
    EventHandler();

    void init(Window& window, Camera& camera, Game& game);

    void processEvents();

    void mouseWheelScrolled(const sf::Event::MouseWheelScrolled* scroll);
private:
    Window* window;

    Camera* camera;

    Game* game;
};