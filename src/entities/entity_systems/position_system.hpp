#pragma once

#include <map>
#include <memory>

class Game;
class Scene;
class EntityLayer;
class Entity;

class PositionSystem
{
public:
    PositionSystem();

    PositionSystem(Game* game, Scene* scene);

    void tick();
private:
    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;

    std::map<int, std::unique_ptr<Entity>>* allEntities;
};