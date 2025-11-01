#pragma once

#include "common.hpp"

class Attribute
{
public:
    Attribute(std::string name);

    std::string getName();
private:
    std::string name;
};