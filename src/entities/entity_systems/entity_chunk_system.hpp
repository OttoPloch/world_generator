#pragma once

#include <vector>
#include <map>
#include <memory>

class Game;
class Scene;
class EntityLayer;
class Entity;

class EntityChunkSystem
{
public:
    EntityChunkSystem();
    
    EntityChunkSystem(Game* game, Scene* scene);

    void entityChunkInit(Entity* entity);

    void tick();
private:
    void tickEntityChunkSync(std::vector<int>& entitiesToUnload, Entity* entity);

    void tickEntityUnload(std::vector<int>& entitiesToUnload, Entity* entity);

    void unloadEntities(std::vector<int> entitiesToUnload);

    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;

    std::map<int, std::unique_ptr<Entity>>* allEntities;
};