#pragma once

#include "tile_tag.hpp"

struct MineableTag : public TileTag
{
    MineableTag(Tile* myTile);
};