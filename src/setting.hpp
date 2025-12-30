#pragma once

#include <string>

struct setting
{
    setting();

    setting(float valueFloat, int valueInt, bool valueBool, std::string valueString);

    void init(float valueFloat, int valueInt, bool valueBool, std::string valueString);

    float valueFloat;
    int valueInt;
    bool valueBool;
    std::string valueString;
};