#include "global_animation.hpp"

GlobalAnimation::GlobalAnimation() : animation(Animation()) {}

GlobalAnimation::GlobalAnimation(Animation animation) : animation(animation) {}

void GlobalAnimation::update(float dt)
{
    animation.secondsTillNextFrame -= dt;

    if (animation.secondsTillNextFrame <= 0)
    {
        animation.index++;

        if (animation.index >= animation.frames.size()) animation.index = 0;
        if (animation.index < 0) animation.index = animation.frames.size() - 1;

        animation.secondsTillNextFrame = animation.secondsPerFrame;
    }
}