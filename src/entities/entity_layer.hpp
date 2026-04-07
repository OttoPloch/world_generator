#pragma once

#include "../core/common.hpp"
#include "entity.hpp"

#include <unordered_map>
#include <vector>
#include <map>
#include <memory>

class Game;

class EntityLayer
{
public:
    EntityLayer();

    void init(Game* game);

    int getNewID();    

    Entity* addEntity(sf::Vector2f position);

    void removeEntity(int ID);

    void removeAllEntitiesInChunk(int chunkX, int chunkY);

    Entity* getEntity(int ID);

    std::vector<Entity*> getEntitiesInChunk(int chunkX, int chunkY);

    void tick();

    void update(float dt);

    void draw(float alpha);
private:
    Game* game;

    int IDCounter;

    std::unordered_map<int, std::unique_ptr<Entity>> entities;
};