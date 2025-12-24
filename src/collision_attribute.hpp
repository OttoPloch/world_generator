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
    CollisionAttribute(Entity* myEntity, EntityStates* states, GamePosition position, sf::Vector2f offset, sf::Vector2f size, std::vector<Entity>* entities, bool active, std::string coliderName);

    void tick();

    void setRect(sf::FloatRect newRect);
    
    CollisionRect* getRect();

    std::string getColliderName();

    bool active;
private:
    Entity* myEntity;

    EntityStates* states;
    
    std::string colliderName;

    CollisionRect rect;
    
    std::vector<Entity>* entities;
};