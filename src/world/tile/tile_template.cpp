#include "tile_template.hpp"

TileTemplate TileTemplate::getCopy()
{
    TileTemplate copy = {type, collides, collOffsetFraction, collSizeFraction, colliderName, myVerts, globalAnimation, nullptr, animSpeedMult, {}};

    if (animation) copy.animation = std::make_unique<Animation>(*animation);

    if (tags.size() > 0)
    {
        for (auto& t : tags)
        {
            copy.tags.emplace_back(t->getCopy());
        }
    }

    return copy;
}