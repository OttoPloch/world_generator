#pragma once

#include "../core/common.hpp"
#include "animation.hpp"

class GlobalAnimation
{
public:
    GlobalAnimation();

    GlobalAnimation(Animation animation);

    void update(float dt);

    Animation animation;
};