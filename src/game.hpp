#pragma once

#include "common.hpp"
#include "window.hpp"
#include "event_handler.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "asset_manager.hpp"

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

    AssetManager assetManager;

    bool paused;
    
    int ticksPerSecond;
    
    sf::Clock dtClock;
    sf::Clock tickClock;

    sf::RectangleShape rect;
    sf::RectangleShape outline;

    Entity thing;
    Entity thing2;
};