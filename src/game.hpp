#pragma once

#include <array>

#include "common.hpp"
#include "window.hpp"
#include "event_handler.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "asset_manager.hpp"
#include "scene.hpp"

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

    void processInput(sf::Keyboard::Key key);
private:
    Window window;

    EventHandler eventHandler;

    AssetManager assetManager;

    Scene scene;

    bool paused;
    
    int ticksPerSecond;
    
    sf::Clock dtClock;
    sf::Clock tickClock;
};