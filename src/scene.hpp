#pragma once

#include "common.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "asset_manager.hpp"
#include "collision_attribute.hpp"
#include "entity_layer.hpp"
#include "ui_layer.hpp"
#include "ui_animation.hpp"
#include "tile_layer.hpp"

#include <vector>

class Game;

class Scene
{
public:
    Scene();

    void init(Game* game);

    void tick();

    void update(float dt);

    void UIUpdate(float dt);

    void draw();

    void sceneInput(std::string control);

    Camera* getCamera();

    void toggleFocus();

    UILayer* getUILayer();

    TileLayer* getTileLayer();
private:
    Game* game;

    Window* window;
    
    AssetManager* assetManager;    

    EntityLayer entityLayer;

    UILayer uiLayer;

    TileLayer tileLayer;

    Camera camera;

    sf::RectangleShape rect;
    sf::RectangleShape outline;
};