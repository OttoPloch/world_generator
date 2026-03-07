#include "animation_set.hpp"
#include "sprite_animation.hpp"
#include "states.hpp"

AnimationSet::AnimationSet() {}

AnimationSet::AnimationSet(std::string setName, std::unordered_map<AnimationState, SpriteAnimation*> animations)
{
    init(setName, animations);
}

void AnimationSet::init(std::string setName, std::unordered_map<AnimationState, SpriteAnimation*> animations)
{
    this->setName = setName;

    this->animations = animations;
}

SpriteAnimation* AnimationSet::getAnimationFor(AnimationState key)
{
    if (animations.find(key) != animations.end())
    {
        return animations[key];
    }
    else
    {
        std::cout << "ERROR loading animation for key of " << static_cast<int>(key) << " in animation set name: " << setName << ". No entry in animations for that key.\n";

        return animations[AnimationState::ANIM_IDLE];
    }
}

int AnimationSet::getKeyFor(SpriteAnimation* animation)
{
    for (auto entry : animations)
    {
        if (entry.second == animation)
        {
            return static_cast<int>(entry.first);
        }
    }

    std::cout << "ERROR in animation set, trying to find key for animation with name of " << animation->getName() << ", no entry in animation map.\n";
    assert(false);

    return -1;
}