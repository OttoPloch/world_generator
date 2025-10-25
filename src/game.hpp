#pragma once

#include "common.hpp"
#include "window.hpp"
#include "event_handler.hpp"
#include "camera.hpp"

class Game
{
public:
    Game();
    
    void init();

    void run();

    void tick();

    void update(float dt);

    void draw();

    void exit();
private:
    Window window;

    EventHandler eventHandler;

    Camera camera;

    bool paused;
    
    int ticksPerSecond;
    
    sf::Clock dtClock;

    sf::RectangleShape rect;
    sf::RectangleShape outline;
};