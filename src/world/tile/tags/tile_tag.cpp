#include "tile_tag.hpp"
#include "../tile.hpp"

std::unique_ptr<TileTag> TileTag::getCopy()
{
    return std::make_unique<TileTag>(*this);
}

void TileTag::onTileDestroy(Tile* tile, bool& endHere)
{

}