#pragma once

#include <array>

#include "common.hpp"
#include "window.hpp"
#include "event_handler.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "../entities/entity.hpp"
#include "../graphics/asset_manager.hpp"
#include "../config/settings.hpp"
#include "../utils/random.hpp"

class Game
{
public:
    Game();
    
    void init();

    void exit();
    
    void processInput(std::string control);
    
    Window* getWindow();

    Settings* getSettings();

    AssetManager* getAssetManager();

    Input* getInput();

    Scene* getScene();

    Random random;
private:
    void run();

    void tick();

    void update();

    void draw();
    
    Window window;
    Settings settings;
    EventHandler eventHandler;
    AssetManager assetManager;
    Input input;
    
    Scene scene;

    bool paused;
    
    int ticksPerSecond;
    float secondsPerTick;
    
    float dt;

    sf::Clock dtClock;
    sf::Clock tickClock;

    sf::Clock gameClock;
    
    // this is to detect window resizing that the
    // event handler doesn't normally catch
    sf::Vector2u lastWindowSize;
    
    // for monitoring performance
    std::unordered_map<std::string, float> runBlame;
    std::unordered_map<std::string, float> drawBlame;
    sf::Clock debugClock;
};