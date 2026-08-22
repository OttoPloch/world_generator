#pragma once

#include <optional>

#include "components/component_data.hpp"

struct EntityTemplate
{
    std::optional<PositionComponentData> position;
    std::optional<SpriteComponentData> sprite;
    std::optional<MovementComponentData> movement;
    std::optional<ControlComponentData> control;
    std::optional<StateComponentData> state;
    std::optional<CollisionComponentData> collision;
    std::optional<ActionComponentData> action;
    std::optional<ItemComponentData> item;
};