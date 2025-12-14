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
    CollisionAttribute(Entity* myEntity, GamePosition position, sf::Vector2f offset, sf::Vector2f size, std::vector<Entity>* entities, bool active);

    void tick();

    CollisionRect getRect();

    bool active;
private:
    Entity* myEntity;
    
    CollisionRect rect;
    
    std::vector<Entity>* entities;
};