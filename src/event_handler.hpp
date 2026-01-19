#pragma once

#include "common.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "scene.hpp"

class Game;

class EventHandler
{
public:
    EventHandler();

    void init(Window* window, Camera* camera, Game* game, Scene* scene);

    void processEvents();

    void mouseWheelScrolled(const sf::Event::MouseWheelScrolled* scroll);

    void windowResized();
private:
    Window* window;

    Camera* camera;

    Game* game;

    Scene* scene;
};