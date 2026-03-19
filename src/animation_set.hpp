#pragma once

#include "common.hpp"
#include "animation.hpp"
#include <SFML/Graphics/Rect.hpp>

class AnimationSet
{
public:
    AnimationSet();

    AnimationSet(std::string setName, std::unordered_map<std::string, Animation> animations);

    void setActiveAnimation(std::string key);

    Animation* getActiveAnimation();
private:
    std::string setName;

    Animation* activeAnimation;

    std::unordered_map<std::string, Animation> animations;
};