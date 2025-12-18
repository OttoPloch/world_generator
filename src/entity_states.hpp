#pragma once

#include "common.hpp"

#include <map>
#include <utility>

class EntityStates
{
public:
    EntityStates();

    void init();

    std::pair<bool, float> getBoth(std::string key);
    
    bool get(std::string key);

    std::string getFirstTrue();

    void set(std::string key, float magnitude = 0.f);

    void resetAll();
private:
    // the pair represents if a value is true or false and it's strength,
    // for example walking(direction) could be true and also have a speed, meanwhile
    // a value like touchingSomething could be true but have no magnitude
    std::map<std::string, std::pair<bool, float>> states;
};