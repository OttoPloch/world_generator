#pragma once

#include "common.hpp"

#include <map>

class Animation;

class AnimationSet
{
public:
    AnimationSet();

    AnimationSet(std::string setName, std::unordered_map<int, Animation*> animations);

    void init(std::string setName, std::unordered_map<int, Animation*> animations);

    Animation* getAnimationFor(int key);

    int getKeyFor(Animation* animation);
private:
    std::string setName;

    std::unordered_map<int, Animation*> animations;
};