#pragma once

#include "common.hpp"

#include <map>
#include <utility>

class EntityStates
{
public:
    EntityStates();

    void init();

    // if applicable, set strength to a value / 1, with the
    // desired speed decreasing as the value goes up since
    // it will be used to multiply the base ticks per frame
    void set(std::string stateSet, int state, float strength = 0.f);

    std::pair<bool, float>* getEntry(std::string stateSet, int state);

    int getFirstTrue(std::string stateSet);

    void resetAll();
private:
    // the pair represents if a value is true or false and it's strength,
    // for example walking(direction) could be true and also have a speed, meanwhile
    // a value like touchingSomething could be true but have no magnitude
    std::unordered_map<std::string, std::map<int, std::pair<bool, float>>> stateMapMap;
};