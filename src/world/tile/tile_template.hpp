#pragma once

#include <optional>
#include <SFML/Graphics.hpp>

#include "tile_types.hpp"
#include "../../graphics/vertex_group.hpp"
#include "../../graphics/global_animation.hpp"
#include "tags/tile_tag.hpp"

struct TileTemplate
{
    TileType type;

    bool collides;
    sf::Vector2f collOffsetFraction;
    sf::Vector2f collSizeFraction;
    std::string colliderName;

    VertexGroup myVerts;

    GlobalAnimation* globalAnimation;
    std::unique_ptr<Animation> animation;
    float animSpeedMult;

    std::vector<std::unique_ptr<TileTag>> tags;
};