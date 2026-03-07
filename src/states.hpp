#pragma once

#include <map>
#include <string>

enum class AnimationState
{
    ANIM_PUSHINGLEFT,
    ANIM_PUSHINGRIGHT,
    ANIM_PUSHINGUP,
    ANIM_PUSHINGDOWN,
    ANIM_RUNNINGLEFT,
    ANIM_RUNNINGRIGHT,
    ANIM_RUNNINGUP,
    ANIM_RUNNINGDOWN,
    ANIM_WALKINGLEFT,
    ANIM_WALKINGRIGHT,
    ANIM_WALKINGUP,
    ANIM_WALKINGDOWN,
    ANIM_MOVING,
    ANIM_IDLE,

    COUNT
};

extern std::map<std::string, AnimationState> animationStringToState;

enum class CollisionState
{
    COLL_ANY,
    COLL_LEFT,
    COLL_RIGHT,
    COLL_TOP,
    COLL_BOTTOM,

    COUNT
};