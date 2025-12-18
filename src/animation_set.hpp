#pragma once

#include "common.hpp"

#include <map>

class Animation;

class AnimationSet
{
public:
    AnimationSet();

    AnimationSet(std::string setName, std::map<std::string, Animation*> animations);

    void init(std::string setName, std::map<std::string, Animation*> animations);

    Animation* getAnimationFor(std::string key);

    std::string getKeyFor(Animation* animation);
private:
    std::string setName;

    std::map<std::string, Animation*> animations;
};