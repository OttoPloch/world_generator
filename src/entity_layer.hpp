#pragma once

#include "common.hpp"
#include "entity.hpp"

#include <vector>
#include <map>
#include <memory>

class Game;

class EntityLayer
{
public:
    EntityLayer();

    void init(Game* game, std::vector<std::unique_ptr<Entity>> entities = {});

    int getNewID();    

    void addEntity(int ID, sf::Vector2f position);

    void removeEntity(int ID);

    // If you are modifying the sprite of this entity, call
    // removeFromZMap() before and addToZMap() after.
    Entity* getEntity(int ID);

    Entity* getPlayer();

    // Only use before modifying entity sprite.
    // Call addToZMap after modification.
    void removeFromZMap(int ID);

    // Only call after calling removeFromZMap
    // and modifying an entity's sprite
    void addToZMap(int ID);

    void giveEntitySprite(int ID, sf::Texture* texture, sf::Vector2f size = {-1.f, -1.f}, int z = 0, bool centerOrigin = true);

    void giveEntityMotion(int ID, float mass, bool controlling = false);

    void giveEntityCollision(int ID, std::string name, int rectType, std::vector<std::string> blacklist = {}, sf::Vector2f offsetFraction = {0, 0}, sf::Vector2f size = {1, 1}, bool sizeIsFraction = true);

    void tick();

    void update(float dt);

    void draw();
private:
    Game* game;

    int IDCounter;

    std::vector<std::unique_ptr<Entity>> entities;

    std::map<int, std::vector<int>> entitiesZMap;

    int playerID;
};