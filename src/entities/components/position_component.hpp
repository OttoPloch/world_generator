#pragma once

#include "entity_component.hpp"
#include "../../utils/game_position.hpp"

struct PositionComponent : public EntityComponent
{
    PositionComponent(Entity* myEntity, GamePosition position);

    GamePosition position;
    sf::Vector2f lastPosition;
};