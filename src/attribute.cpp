#include "attribute.hpp"

Attribute::Attribute(std::string name)
{
    this->name = name;
}

std::string Attribute::getName() { return name; }