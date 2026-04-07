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

    std::vector<Entity*> getEntitiesInChunk(int chunkX, int chunkY);

    void tick();

    void update(float dt);

    void draw(float alpha);
private:
    Game* game;

    TemplateManager tManager;

    int IDCounter;

    std::unordered_map<int, std::unique_ptr<Entity>> entities;
};