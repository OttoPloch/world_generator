#pragma once

#include "tile_tag.hpp"

struct MineableTag : public TileTag
{
    MineableTag(float durability);

    std::unique_ptr<TileTag> getCopy() override;

    float durability;
};