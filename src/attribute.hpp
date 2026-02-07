#pragma once

#include "common.hpp"

class Attribute
{
public:
    Attribute(std::string attributeName);

    std::string getAttributeName();
private:
    std::string attributeName;
};