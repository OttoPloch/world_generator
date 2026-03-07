#include "entity_states.hpp"
#include "states.hpp"

std::map<std::string, AnimationState> animationStringToState = {
    {"ANIM_PUSHINGLEFT", AnimationState::ANIM_PUSHINGLEFT},
    {"ANIM_PUSHINGRIGHT", AnimationState::ANIM_PUSHINGRIGHT},
    {"ANIM_PUSHINGUP", AnimationState::ANIM_PUSHINGUP},
    {"ANIM_PUSHINGDOWN", AnimationState::ANIM_PUSHINGDOWN},
    {"ANIM_RUNNINGLEFT", AnimationState::ANIM_RUNNINGLEFT},
    {"ANIM_RUNNINGRIGHT", AnimationState::ANIM_RUNNINGRIGHT},
    {"ANIM_RUNNINGUP", AnimationState::ANIM_RUNNINGUP},
    {"ANIM_RUNNINGDOWN", AnimationState::ANIM_RUNNINGDOWN},
    {"ANIM_WALKINGLEFT", AnimationState::ANIM_WALKINGLEFT},
    {"ANIM_WALKINGRIGHT", AnimationState::ANIM_WALKINGRIGHT},
    {"ANIM_WALKINGUP", AnimationState::ANIM_WALKINGUP},
    {"ANIM_WALKINGDOWN", AnimationState::ANIM_WALKINGDOWN},
    {"ANIM_MOVING", AnimationState::ANIM_MOVING},
    {"ANIM_IDLE", AnimationState::ANIM_IDLE}
};

EntityStates::EntityStates() { init(); }

void EntityStates::init()
{
    for (int i = 0; i < static_cast<int>(AnimationState::COUNT); i++)
    {
        animationStates[static_cast<AnimationState>(i)] = {false, 0.f};
    }

    for (int i = 0; i < static_cast<int>(CollisionState::COUNT); i++)
    {
        collisionStates[static_cast<CollisionState>(i)] = {false, 0.f};
    }
}

void EntityStates::set(std::string stateSet, AnimationState state, float strength)
{
    std::pair<bool, float>* pair = getEntry(stateSet, static_cast<int>(state));

    *pair = {true, strength};
}

std::pair<bool, float>* EntityStates::getEntry(std::string stateSet, int state)
{
    if (stateSet == "animation")
    {
        auto entry = animationStates.find(static_cast<AnimationState>(state));

        if (entry != animationStates.end())
        {
            return &animationStates[static_cast<AnimationState>(state)];
        }
        else
        {
            std::cout << "ERROR could not get state of " << state << " from set " << stateSet << ", that state was not found.\n";
        }
    }
    else if (stateSet == "collision")
    {
        auto entry = collisionStates.find(static_cast<CollisionState>(state));

        if (entry != collisionStates.end())
        {
            return &collisionStates[static_cast<CollisionState>(state)];
        }
        else
        {
            std::cout << "ERROR could not get state of " << state << " from set " << stateSet << ", that state was not found.\n";
        }
    }
    else
    {
        std::cout << "ERROR could not get state of " << state << " from set " << stateSet << ", that set was not found.\n";
    }

    return nullptr;
}

int EntityStates::getFirstTrue(std::string stateSet)
{
    if (stateSet == "animation")
    {
        for (auto entry : animationStates)
        {
            if (entry.second.first) return static_cast<int>(entry.first);
        }
    }
    else if (stateSet == "collision")
    {
        for (auto entry : collisionStates)
        {
            if (entry.second.first) return static_cast<int>(entry.first);
        }
    }
    else
    {
        std::cout << "ERROR could not first true from set " << stateSet << ", that set was not found.\n";
    }

    return -1;
}

void EntityStates::resetAll()
{
    for (auto& entry : animationStates)
    {
        (entry.first != AnimationState::ANIM_IDLE) ? entry.second = {false, 0.f} : entry.second = {true, 0.f};
    }

    for (auto& entry : collisionStates)
    {
        entry.second = {false, 0.f};
    }
}