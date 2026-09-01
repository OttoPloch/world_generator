#pragma once

#include <vector>

class Game;
class Scene;
class EntityLayer;
class Entity;

class RenderSystem
{
public:
    RenderSystem();

    RenderSystem(Game* game, Scene* scene);

    void update(float dt);

    void draw(bool debug);

    void refactorEntityCache();
private:
    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;

    std::vector<Entity*> validEntities;
};