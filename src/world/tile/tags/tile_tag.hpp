#pragma once

#include <string>

class Tile;

struct TileTag
{
    TileTag(std::string name);

    virtual ~TileTag();

    std::string name;
};