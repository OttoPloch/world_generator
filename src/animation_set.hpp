#pragma once

#include "common.hpp"

#include <map>

class SpriteAnimation;

class AnimationSet
{
public:
    AnimationSet();

    AnimationSet(std::string setName, std::unordered_map<int, SpriteAnimation*> animations);

    void init(std::string setName, std::unordered_map<int, SpriteAnimation*> animations);

    SpriteAnimation* getAnimationFor(int key);

    int getKeyFor(SpriteAnimation* animation);
private:
    std::string setName;

    std::unordered_map<int, SpriteAnimation*> animations;
};