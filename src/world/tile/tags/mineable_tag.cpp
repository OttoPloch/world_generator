#include "mineable_tag.hpp"
#include "tile_tag.hpp"

MineableTag::MineableTag(float durability) : TileTag("MINE"), durability(durability) {}

std::unique_ptr<TileTag> MineableTag::getCopy()
{
    return std::make_unique<MineableTag>(*this);
}