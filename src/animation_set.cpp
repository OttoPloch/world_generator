#include "animation_set.hpp"
#include "animation.hpp"
#include <string>
#include <unordered_map>

AnimationSet::AnimationSet() {}

AnimationSet::AnimationSet(std::string setName, std::unordered_map<AnimState, Animation> animations)
{
    this->setName = setName;
    this->animations = animations;

    activeAnimation = nullptr;
}

void AnimationSet::setActiveAnimation(AnimState key)
{
    if (activeAnimation)
    {
        activeAnimation->secondsTillNextFrame = activeAnimation->secondsPerFrame;
        activeAnimation->index = 0;
    }

    auto entry = animations.find(key);

    if (entry != animations.end())
    {
        activeState = key;

        activeAnimation = &entry->second;
        activeAnimation->secondsTillNextFrame = 0.f;
    }
}

Animation* AnimationSet::getActiveAnimation() { return activeAnimation; }

AnimState AnimationSet::getActiveState() { return activeState; }