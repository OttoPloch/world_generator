#pragma once

#include "common.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "asset_manager.hpp"
#include "collision_attribute.hpp"
#include "entity_layer.hpp"
#include "ui_layer.hpp"
#include "ui_animation.hpp"
#include "chunk_layer.hpp"

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

    void chunkUpdate();

    void draw();

    void sceneInput(std::string control);

    Camera* getCamera();

    void toggleFocus();

    UILayer* getUILayer();

    ChunkLayer* getChunkLayer();
private:
    Game* game;

    Window* window;
    
    AssetManager* assetManager;    

    EntityLayer entityLayer;

    UILayer uiLayer;

    ChunkLayer chunkLayer;

    Camera camera;

    sf::RectangleShape rect;
    sf::RectangleShape outline;
};