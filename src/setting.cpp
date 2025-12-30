#include "setting.hpp"

setting::setting() {}

setting::setting(float valueFloat, int valueInt, bool valueBool, std::string valueString)
{
    init(valueFloat, valueInt, valueBool, valueString);
}

void setting::init(float valueFloat, int valueInt, bool valueBool, std::string valueString)
{
    this->valueFloat = valueFloat;

    this->valueInt = valueInt;

    this->valueBool = valueBool;

    this->valueString = valueString;
}