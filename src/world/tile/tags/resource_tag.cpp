#include "resource_tag.hpp"
#include "../tile.hpp"
#include "../../../core/game.hpp"
#include <iostream>

ResourceTag::ResourceTag(std::string resource, int amountBase, int amountRange) : resource(resource), amountBase(amountBase), amountRange(amountRange)
{

}

std::unique_ptr<TileTag> ResourceTag::getCopy()
{
    return std::make_unique<ResourceTag>(*this);
}

void ResourceTag::onTileDestroy(Tile* tile, bool& endHere)
{
    int extra = tile->game->random.getRandInt(0, amountRange);
    int total = amountBase + extra;

    /////// TEMP ///////
    std::cout << "dropping " << total << " (base " << amountBase << ", range " << amountRange << ", extra " << extra << ") " << resource << '\n';
    ////////////////////
}