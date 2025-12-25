#pragma once

#include <vector>
#include <utility>

#include "common.hpp"
#include "attribute.hpp"
#include "game_position.hpp"
#include "collision_rect.hpp"
#include "entity.hpp"

class CollisionAttribute : public Attribute
{
public:
    CollisionAttribute(Entity* myEntity, EntityStates* states, GamePosition position, sf::Vector2f offset, sf::Vector2f size, std::vector<Entity>* entities, std::string colliderName, int rectType, std::vector<std::string> blacklist);

    void tick();

    bool collidesWith(CollisionRect* other);

    void resolveCollision(CollisionRect* other, float pushFraction);

    void setRect(sf::FloatRect newRect);
    
    CollisionRect* getRect();
private:
    Entity* myEntity;

    EntityStates* states;
    
    CollisionRect rect;
    
    std::vector<Entity>* entities;
};