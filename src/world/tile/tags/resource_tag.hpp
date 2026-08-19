#pragma once

#include "tile_tag.hpp"

struct ResourceTag : public TileTag
{
    ResourceTag(std::string resource, int amountBase, int amountRange);

    std::unique_ptr<TileTag> getCopy() override;

    void onTileDestroy(Tile* tile, bool& endHere) override;

    std::string resource;
    int amountBase; // the lowest amount of resources this tile can drop.
    int amountRange; // the max possible additional resources the tile can drop on top of amountBase.
};