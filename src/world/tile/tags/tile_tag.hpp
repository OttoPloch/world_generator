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

    // What to do when the tile this tag is applied to is destroyed (does not apply when Chunk::setTile() is used).
    // If endHere is set to true, the tile will stop iterating through its tags and calling this function.
    virtual void onTileDestroy(Tile* tile, bool& endHere);
};