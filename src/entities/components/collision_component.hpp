#pragma once

#include "../../core/common.hpp"
#include "entity_component.hpp"
#include "../collision_rect.hpp"

struct CollisionComponent : EntityComponent
{
    CollisionComponent(Entity* myEntity, GamePosition position, sf::Vector2f size, bool sizeIsScaleOfSprite, RectType type);

    CollisionRect rect;
};