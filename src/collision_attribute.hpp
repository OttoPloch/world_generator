#pragma once

#include "common.hpp"
#include "attribute.hpp"
#include "game_position.hpp"
#include "collision_rect.hpp"
#include "entity.hpp"
#include "tile.hpp"

#include <vector>
#include <utility>
#include <memory>

class CollisionAttribute : public Attribute
{
public:
    CollisionAttribute();

    CollisionAttribute(Entity* myEntity, GamePosition position, sf::Vector2f offset, sf::Vector2f size, std::string colliderName, int rectType, std::vector<std::string> blacklist);

    void tick(char axis, std::vector<std::unique_ptr<Entity>>* entities = nullptr, std::vector<std::vector<Tile>*> surroundingTiles = {});

    std::vector<Entity*> getEntityCollisions(std::vector<std::unique_ptr<Entity>>* entities);

    std::vector<Tile*> getTileCollisions(std::vector<std::vector<Tile>*>* surroundingTiles);

    bool collidesWith(CollisionRect other);

    bool collidesWith(sf::FloatRect other);

    void resolveCollision(char axis, CollisionRect* other, sf::Vector2f relMove, float myMass, float otherMass);
    
    void resolveCollision(char axis, sf::FloatRect other);

    void setRect(sf::FloatRect newRect);
    
    CollisionRect* getRect();

    Entity* getEntity();
private:
    Entity* myEntity;

    EntityStates* states;
    
    CollisionRect rect;
};