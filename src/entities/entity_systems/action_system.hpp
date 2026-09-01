#pragma once

#include <vector>

class Game;
class Scene;
class EntityLayer;
class Entity;

class ActionSystem
{
public:
    ActionSystem();

    ActionSystem(Game* game, Scene* scene);

    void update(float dt);

    void refactorEntityCache();
private:
    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;

    std::vector<Entity*> validEntities;
};