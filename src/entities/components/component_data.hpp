#pragma once

#include "../../core/common.hpp"
#include "../../graphics/animation.hpp"
#include "../../graphics/animation_set.hpp"
#include "../rect_type.hpp"
#include "../actions/action.hpp"

struct SpriteData
{
    sf::Texture* texture;
    sf::Vector2f size;
    bool sizeIsScale;
    bool usingTexCoords;
    sf::IntRect texCoords;
    float animSpeedMult;
    Animation* animation;
    AnimationSet* animSet;
};

struct MovementComponentData
{
    float speed;
    float sprintMultilpier;
};

struct ControlComponentData
{

};

struct StateComponentData
{

};

struct CollisionComponentData
{
    sf::Vector2f size;
    bool sizeIsScaleOfSprite;
    RectType type;
};

struct ActionComponentData
{
    std::unique_ptr<Action> mainAction;
    std::unique_ptr<Action> secondaryAction;

    float range;
};