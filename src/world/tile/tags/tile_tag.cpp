#include "tile_tag.hpp"
#include "../tile.hpp"

TileTag::TileTag(std::string name) : name(name) {}

std::unique_ptr<TileTag> TileTag::getCopy()
{
    return std::make_unique<TileTag>(*this);
}