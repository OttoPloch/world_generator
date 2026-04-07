#pragma once

#include "../../core/common.hpp"
#include "entity_component.hpp"
#include "component_data.hpp"

struct MovementComponent : public EntityComponent
{
    MovementComponent(Entity* myEntity, sf::Vector2f velocity, MovementComponentData stats);

    void tick() override;

    void update() override;

    sf::Vector2f velocity;

    MovementComponentData stats;
};