#include "entity_states.hpp"

EntityStates::EntityStates() {}

void EntityStates::init()
{
    states = {
    {"idle", {true, 0.f}},
    {"walkingLeft", {false, 0.f}},
    {"walkingRight", {false, 0.f}},
    {"walkingUp", {false, 0.f}},
    {"walkingDown", {false, 0.f}},
    {"walking", {false, 0.f}},
    {"touchingSomething", {false, 0.f}},
    {"touchingLeft", {false, 0.f}},
    {"touchingRight", {false, 0.f}},
    {"touchingUp", {false, 0.f}},
    {"touchingDown", {false, 0.f}}
    };
}

std::pair<bool, float> EntityStates::getBoth(std::string key)
{
    if (states.find(key) != states.end())
    {
        return states[key];
    }
    else
    {
        std::cout << "error with entity states, tried to get pair from key " << key << ", which doesn't exist\n";
        assert(false);
    }
}

bool EntityStates::get(std::string key)
{
    if (states.find(key) != states.end())
    {
        return states[key].first;
    }
    else
    {
        std::cout << "error with entity states, tried to get bool from key " << key << ", which doesn't exist\n";
        assert(false);
    }
}

std::string EntityStates::getFirstTrue()
{
    // TODO: fix this and kinda fix the whole animation trigger system
    if (states["idle"].first) return "idle";
    if (states["walkingLeft"].first) return "walkingLeft";
    if (states["walkingRight"].first) return "walkingRight";
    if (states["walkingUp"].first) return "walkingUp";
    if (states["walkingDown"].first) return "walkingDown";

    return "NONE";
}

void EntityStates::set(std::string key, float magnitude)
{
    auto entry = states.find(key);

    if (entry != states.end())
    {
        entry->second = {true, magnitude};

        states["idle"] = {false, 0.f};
    }
    else
    {
        std::cout << "WARNING: entity states, setting key of " << key << " to true with a magnitude of " << magnitude << ". THIS KEY DOES NOT EXIST.\n";
    }
}

void EntityStates::resetAll()
{
    for (auto& entry : states)
    {
        (entry.first != "idle") ? entry.second = {false, 0.f} : entry.second = {true, 0.f};
    }
}