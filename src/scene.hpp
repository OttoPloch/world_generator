#pragma once

#include "common.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "asset_manager.hpp"

class Scene
{
public:
    Scene();

    void init(Window* window, AssetManager* assetManager);

    void tick();

    void update(float dt);

    void draw();

    void sceneInput(sf::Keyboard::Key key);

    Camera* getCamera();

    void toggleFocus();
private:
    Window* window;
    
    AssetManager* assetManager;    

    Camera camera;

    sf::RectangleShape rect;
    sf::RectangleShape outline;

    Entity thing;
    Entity thing2;
};