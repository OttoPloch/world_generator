#pragma once

#include "common.hpp"
#include "states.hpp"

#include <map>

class SpriteAnimation;

class AnimationSet
{
public:
    AnimationSet();

    AnimationSet(std::string setName, std::unordered_map<AnimationState, SpriteAnimation*> animations);

    void init(std::string setName, std::unordered_map<AnimationState, SpriteAnimation*> animations);

    SpriteAnimation* getAnimationFor(AnimationState key);

    int getKeyFor(SpriteAnimation* animation);
private:
    std::string setName;

    std::unordered_map<AnimationState, SpriteAnimation*> animations;
};