#include "tile.hpp"
#include "../../core/game.hpp"
#include "../chunk/chunk.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <array>

Tile::Tile() {}

Tile::Tile(Game* game, Chunk* chunk, sf::Vector2i localPosition, const TileTemplate& t, int z)
{
    this->game = game;
    this->chunk = chunk;
    this->localPosition = localPosition;
    size = game->getSettings()->tile_size;

    type = t.type;
    collides = t.collides;
    collOffsetFraction = t.collOffsetFraction;
    collSizeFraction = t.collSizeFraction;
    colliderName = t.colliderName;
    myVerts.texCoords = t.myVerts.texCoords;
    globalAnimation = t.globalAnimation;
    if (t.animation) animation = std::make_unique<Animation>(*t.animation);
    animSpeedMult = t.animSpeedMult;
    for (auto& t : t.tags) tags.emplace_back(t->getCopy());
    
    this->z = z;
}

void Tile::update(float dt)
{
    if (globalAnimation)
    {
        if (globalAnimation->animation.name != "")
        {
            sf::FloatRect newTexCoords = globalAnimation->animation.frames[globalAnimation->animation.index];
    
            if (myVerts.texCoords != newTexCoords)
            {
                myVerts.texCoords = newTexCoords;
                chunk->createTileVerts(localPosition, z);
            }
        }
    }
    else if (animation)
    {
        animation->secondsTillNextFrame -= dt * animSpeedMult;

        if (animation->secondsTillNextFrame <= 0.f)
        {
            (animation->reversed) ? animation->index-- : animation->index++;
            
            if (animation->index >= animation->frames.size()) animation->index = 0;
            if (animation->index < 0) animation->index = animation->frames.size() - 1;
            
            sf::FloatRect newTexCoords = animation->frames[animation->index];

            if (myVerts.texCoords != newTexCoords)
            {
                myVerts.texCoords = newTexCoords;
                chunk->createTileVerts(localPosition, z);
            }

            animation->secondsTillNextFrame = animation->secondsPerFrame;
        }
    }
}