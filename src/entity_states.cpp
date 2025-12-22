#include "entity_states.hpp"
#include "states.hpp"

EntityStates::EntityStates() { init(); }

void EntityStates::init()
{
    stateMapMap = {
        {"animation", {
            {ANIM_WALKINGLEFT, {false, 0.f}},
            {ANIM_WALKINGRIGHT, {false, 0.f}},
            {ANIM_WALKINGUP, {false, 0.f}},
            {ANIM_WALKINGDOWN, {false, 0.f}},
            {ANIM_MOVING, {false, 0.f}},
            {ANIM_IDLE, {true, 0.f}}
        }},
        {"collision", {
            {COLL_ANY, {false, 0.f}},
            {COLL_LEFT, {false, 0.f}},
            {COLL_RIGHT, {false, 0.f}},
            {COLL_TOP, {false, 0.f}},
            {COLL_BOTTOM, {false, 0.f}}
        }}
    };
}

void EntityStates::set(std::string stateSet, int state, float strength)
{
    std::pair<bool, float>* pair = getEntry(stateSet, state);

    *pair = {true, strength};
}

std::pair<bool, float>* EntityStates::getEntry(std::string stateSet, int state)
{
    auto mapEntry = stateMapMap.find(stateSet);

    if (mapEntry != stateMapMap.end())
    {
        auto* map = &mapEntry->second;

        auto stateEntry = map->find(state);

        if (stateEntry != map->end())
        {
            return &(*map)[state];
        }
        else
        {
            std::cout << "ERROR getting state with key of " << state << " from state set " << stateSet << ". That state does not exist.\n";
            assert(false);
        }
    }
    else
    {
        std::cout << "ERROR getting state with key of " << state << " from stateSet " << stateSet << ". The given state set does not exist.\n";
        assert(false);
    }

    return nullptr;
}

int EntityStates::getFirstTrue(std::string stateSet)
{
    auto mapEntry = stateMapMap.find(stateSet);

    if (mapEntry != stateMapMap.end())
    {
        auto map = mapEntry->second;

        for (auto stateEntry : map)
        {
            if (stateEntry.second.first)
            {
                return stateEntry.first;
            }
        }
    }
    else
    {
        std::cout << "ERROR getting first true from state set " << stateSet << ". The given state set does not exist.\n";
        assert(false);
    }

    return -1;
}

void EntityStates::resetAll()
{
    for (auto& entry : stateMapMap["animation"])
    {
        (entry.first != ANIM_IDLE) ? entry.second = {false, 0.f} : entry.second = {true, 0.f};
    }

    for (auto& entry : stateMapMap["collision"])
    {
        entry.second = {false, 0.f};
    }
}