#pragma once

#include "common.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "asset_manager.hpp"
#include "collision_attribute.hpp"
#include "entity_layer.hpp"
#include "ui_layer.hpp"

#include <vector>

class Game;

class Scene
{
public:
    Scene();

    void init(Game* game);

    void tick();

    void update(float dt);

    void draw();

    void sceneInput(sf::Keyboard::Key key);

    Camera* getCamera();

    void toggleFocus();

    int getNewID();

    UILayer* getUILayer();
private:
    Window* window;
    
    AssetManager* assetManager;    

    int IDCounter;
    
    EntityLayer entityLayer;

    UILayer uiLayer;

    Camera camera;

    sf::RectangleShape rect;
    sf::RectangleShape outline;
};