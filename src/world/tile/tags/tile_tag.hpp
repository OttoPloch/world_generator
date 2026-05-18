#pragma once

#include <string>
#include <memory>

class Tile;

struct TileTag
{
    // returns a new object, a direct copy
    // of this one, including any child classes
    // so long as you make an override for them
    virtual std::unique_ptr<TileTag> getCopy();
};