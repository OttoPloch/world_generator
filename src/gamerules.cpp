#include "gamerules.hpp"

Gamerules::Gamerules()
{
    init();
}

void Gamerules::init()
{
    float bigZoomFactor = 1.5f;

    rules = {
        {"camera_freecamMoveSpeedBase", gamerule(0.f, 10, false, "")},
        {"camera_freecamFriction", gamerule(8.f, 0, false, "")},
        {"camera_bigZoomFactor", gamerule(bigZoomFactor, 0, false, "")},
        {"camera_smallZoomAmount", gamerule(0.1f, 0, false, "")},
        {"camera_minZoomFactor", gamerule(0.1f, 0, false, "")},
        {"camera_maxZoomFactor", gamerule(pow(bigZoomFactor, 8), 0, false, "")},
        {"camera_focusFollowDelay", gamerule(20.f, 0, false, "")},
        {"sprite_followDelay", gamerule(3.f, 0, false, "")},
        {"motion_friction", gamerule(0.8f, 0, false, "")},
        {"motion_velocityCutoff", gamerule(0.01f, 0, false, "")}
    };

    playerRules = {
        {"moveSpeed", gamerule(5.f, 0, false, "")},
        {"acceleration", gamerule(5.f, 0, false, "")}
    };
}

gamerule Gamerules::getRule(std::string rule, std::string ruleset)
{
    if (ruleset == "default")
    {
        if (rules.find(rule) != rules.end())
        {
            return rules[rule];
        }
    }
    else if (ruleset == "player")
    {
        if (playerRules.find(rule) != playerRules.end())
        {
            return playerRules[rule];
        }
    }

    std::cout << "ERROR finding gamerule " << rule << " from ruleset " << ruleset << ". Rule does not exist in that set.\n";

    assert(false);
    
    return gamerule(0.f, 0, false, "");
}

void Gamerules::setRule(std::string rule, gamerule value, std::string ruleset)
{
    if (ruleset == "default")
    {
        if (rules.find(rule) != rules.end())
        {
            rules[rule] = value;
        }
    }
    else if (ruleset == "player")
    {
        if (playerRules.find(rule) != playerRules.end())
        {
            playerRules[rule] = value;
        }
    }
}