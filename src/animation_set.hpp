#pragma once

#include "common.hpp"
#include "animation.hpp"
#include "states.hpp"

#include <SFML/Graphics/Rect.hpp>

class AnimationSet
{
public:
    AnimationSet();

    AnimationSet(std::string setName, std::unordered_map<AnimState, Animation> animations);

    void setActiveAnimation(AnimState key);

    Animation* getActiveAnimation();
    AnimState getActiveState();
private:
    std::string setName;

    Animation* activeAnimation;
    AnimState activeState;

    std::unordered_map<AnimState, Animation> animations;
};