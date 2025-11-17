#pragma once

#include "common.hpp"
#include "camera.hpp"
#include "window.hpp"

class Scene
{
public:
    Scene();

    void init(Window* window, AssetManager* assetManager);

    void tick();

    void update(float dt);

    void draw(sf::RenderWindow* window);

    Camera* getCamera();
private:
    Camera camera;

    sf::RectangleShape rect;
    sf::RectangleShape outline;

    Entity thing;
    Entity thing2;
};