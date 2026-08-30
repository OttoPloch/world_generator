#pragma once

#include <vector>

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

    void unloadEntity(int ID);

    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;
};