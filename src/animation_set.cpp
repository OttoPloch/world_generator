#include "animation_set.hpp"
#include "animation.hpp"

AnimationSet::AnimationSet() {}

AnimationSet::AnimationSet(std::string setName, std::map<std::string, Animation*> animations)
{
    init(setName, animations);
}

void AnimationSet::init(std::string setName, std::map<std::string, Animation*> animations)
{
    this->setName = setName;

    this->animations = animations;
}

Animation* AnimationSet::getAnimationFor(std::string key)
{
    if (animations.find(key) != animations.end())
    {
        return animations[key];
    }
    else
    {
        std::cout << "ERROR loading animation for key of " << key << " in animation set name: " << setName << ". No entry in animations for that key.\n";

        return nullptr;
    }
}

std::string AnimationSet::getKeyFor(Animation* animation)
{
    for (auto entry : animations)
    {
        if (entry.second == animation)
        {
            return entry.first;
        }
    }

    std::cout << "ERROR in animation set, trying to find key for animation with name of " << animation->getName() << ", no entry in animation map.\n";
    assert(false);

    return "is this return necessary?";
}