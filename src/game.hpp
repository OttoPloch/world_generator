#pragma once

#include <array>

#include "common.hpp"
#include "window.hpp"
#include "event_handler.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "asset_manager.hpp"
#include "scene.hpp"
#include "gamerules.hpp"

class Game
{
public:
    Game();
    
    void init();

    void exit();
    
    void processInput(sf::Keyboard::Key key);
    
    Window* getWindow();
    
    Gamerules* getGamerules();

    AssetManager* getAssetManager();
private:
    void run();

    void tick();

    void update(float dt);

    void draw();
    
    Window window;

    Gamerules gamerules;

    EventHandler eventHandler;

    AssetManager assetManager;

    Scene scene;

    bool paused;
    
    int ticksPerSecond;
    
    float dt;

    sf::Clock dtClock;
    sf::Clock tickClock;
    sf::Clock gameClock;
};