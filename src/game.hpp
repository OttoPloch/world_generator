#pragma once

#include "window.hpp"
#include "event_handler.hpp"

class Game
{
public:
    Game();
    
    void init();

    void run();

    void tick();

    void update();

    void draw();

    void exit();
private:
    Window window;

    EventHandler eventHandler;

    bool paused;
    
    int ticksPerSecond;
    
    sf::Clock dtClock;

    sf::RectangleShape rect;
};