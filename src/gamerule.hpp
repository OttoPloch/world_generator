#pragma once

#include <string>

struct gamerule
{
    gamerule();

    gamerule(float valueFloat, int valueInt, bool valueBool, std::string valueString);

    void init(float valueFloat, int valueInt, bool valueBool, std::string valueString);

    float valueFloat;
    int valueInt;
    bool valueBool;
    std::string valueString;
};