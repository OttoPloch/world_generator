#pragma once

#include "common.hpp"
#include "entity_component.hpp"

struct MovementComponent : public EntityComponent
{
    MovementComponent(Entity* myEntity, sf::Vector2f velocity, float speed = 30.f);

    void tick() override;

    void update() override;

    sf::Vector2f velocity;

    float speed;
};