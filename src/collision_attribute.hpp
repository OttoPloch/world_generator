#pragma once

#include <vector>

#include "common.hpp"
#include "attribute.hpp"
#include "game_position.hpp"
#include "collision_rect.hpp"
#include "entity.hpp"

class CollisionAttribute : public Attribute
{
public:
    CollisionAttribute(int myEntityID, GamePosition position, sf::Vector2f size, std::vector<Entity>* entities, bool active);

    void tick();

    bool active;
private:
    int myEntityID;
    
    GamePosition position;
    
    sf::Vector2f size;
    
    CollisionRect rect;
    
    std::vector<Entity>* entities;
};