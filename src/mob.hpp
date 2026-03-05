#pragma once

#include "common.hpp"
#include "entity.hpp"

class Mob : public Entity
{
public:
    Mob();

    Mob(Game* game, int ID, sf::Vector2f position);

    Mob(Game* game, int ID, WorldPosition position);

    void tick() override;

    sf::Vector2f velocity;
};