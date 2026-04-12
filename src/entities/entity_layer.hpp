#pragma once

#include <unordered_map>
#include <vector>
#include <map>
#include <memory>

#include "../core/common.hpp"
#include "entity.hpp"
#include "entity_template.hpp"
#include "template_manager.hpp"

class Game;

class EntityLayer
{
public:
    EntityLayer();

    void init(Game* game);

    int getNewID();    

    Entity* addEntity(sf::Vector2f position, EntityTemplate* t = nullptr);

    void removeEntity(int ID);

    void removeAllEntitiesInChunk(int chunkX, int chunkY);

    Entity* getEntity(int ID);

    // chunkRadius controls the distance to scan for entities.
    // The default of 0 only scans in the chunk position given.
    std::vector<Entity*> getEntitiesInChunkArea(int chunkX, int chunkY, int chunkRadius = 0);
    std::vector<Entity*> getEntitiesInChunkArea(sf::Vector2f position, int chunkRadius = 0);

    void tick();

    void update(float dt);

    void draw();
private:
    Game* game;

    TemplateManager tManager;

    int IDCounter;

    std::map<int, std::unique_ptr<Entity>> entities;
};