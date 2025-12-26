#pragma once

#include "common.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "asset_manager.hpp"
#include "collision_attribute.hpp"

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
private:
    Window* window;
    
    AssetManager* assetManager;    

    int IDCounter;
    
    std::vector<Entity> entities;

    std::map<int, std::vector<Entity*>> entitiesZMap;

    Camera camera;

    sf::RectangleShape rect;
    sf::RectangleShape outline;
};