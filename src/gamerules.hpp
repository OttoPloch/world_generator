#pragma once

#include "common.hpp"
#include "gamerule.hpp"

#include <map>

class Gamerules
{
public:
    Gamerules();

    void init();

    gamerule getRule(std::string rule, std::string ruleset = "default");
    
    void setRule(std::string rule, gamerule value, std::string ruleset = "default");
private:
    std::map<std::string, gamerule> rules;

    std::map<std::string, gamerule> playerRules;
};