#pragma once

#include <vector>

class Game;
class Scene;
class EntityLayer;
class Entity;

class EntityUISystem
{
public:
    EntityUISystem();

    EntityUISystem(Game* game, Scene* scene);

    void tick();

    void refactorEntityCache();
private:
    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;

    std::vector<Entity*> validEntities;
};