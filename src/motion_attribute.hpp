#pragma once

#include "common.hpp"
#include "attribute.hpp"
#include "game_position.hpp"
#include "entity_states.hpp"
#include "gamerules.hpp"

class Game;

class MotionAttribute : public Attribute
{
public:
    MotionAttribute(Game* game, Entity* myEntity, GamePosition position, float mass, bool controlling);

    void tick();

    sf::Vector2f getVelocity();

    void setVelocity(sf::Vector2f newVelocity);

    void setVelocity(char direction, float newVelocity);
    
    void changeVelocity(sf::Vector2f amount);

    void changeVelocity(char direction, float amount);
    
    float getRotation();

    float getMass();

    bool controlling;
private:
    Game* game;

    Entity* myEntity;

    Gamerules* gamerules;

    EntityStates* states;

    GamePosition position;

    float rotation;

    sf::Vector2f velocity;

    float rotationalVelocity;

    float mass;
};