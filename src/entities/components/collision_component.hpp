#pragma once

#include "../../core/common.hpp"
#include "entity_component.hpp"
#include "../collision_rect.hpp"

struct CollisionComponent : EntityComponent
{
    CollisionComponent(Entity* myEntity, WorldPosition position, sf::Vector2f size, bool sizeIsScaleOfSprite, RectType type);

    void update() override;

    void resolveCollision(CollisionRect* other, char direction, float pushFraction);

    CollisionRect rect;
};