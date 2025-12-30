#pragma once

#include "common.hpp"
#include "attribute.hpp"
#include "game_position.hpp"
#include "collision_rect.hpp"
#include "entity.hpp"

#include <vector>
#include <utility>
#include <memory>

class CollisionAttribute : public Attribute
{
public:
    CollisionAttribute(Entity* myEntity, GamePosition position, sf::Vector2f offset, sf::Vector2f size, std::vector<std::unique_ptr<Entity>>* entities, std::string colliderName, int rectType, std::vector<std::string> blacklist);

    void tick();

    bool collidesWith(CollisionRect* other);

    void resolveCollision(Entity* otherEntity, float pushFraction);

    void setRect(sf::FloatRect newRect);
    
    CollisionRect* getRect();

    Entity* getEntity();
private:
    Entity* myEntity;

    EntityStates* states;
    
    CollisionRect rect;
    
    std::vector<std::unique_ptr<Entity>>* entities;
};