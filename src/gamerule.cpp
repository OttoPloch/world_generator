#include "gamerule.hpp"

gamerule::gamerule() {}

gamerule::gamerule(float valueFloat, int valueInt, bool valueBool, std::string valueString)
{
    init(valueFloat, valueInt, valueBool, valueString);
}

void gamerule::init(float valueFloat, int valueInt, bool valueBool, std::string valueString)
{
    this->valueFloat = valueFloat;

    this->valueInt = valueInt;

    this->valueBool = valueBool;

    this->valueString = valueString;
}