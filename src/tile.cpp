#include "tile.hpp"
#include "game.hpp"
#include "chunk.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <array>

Tile::Tile() {}

Tile::Tile(TileType type, sf::IntRect texCoords, bool collides, std::string colliderName, sf::Vector2f collOffsetFraction, sf::Vector2f collSizeFraction)
{
    this->type = type;
    myVerts.texCoords = texCoords;
    this->collides = collides;
    this->colliderName = colliderName;
    this->collOffsetFraction = collOffsetFraction;
    this->collSizeFraction = collSizeFraction;
}

Tile::Tile(Game* game, Chunk* chunk, sf::Vector2i localPosition, TileType type, sf::IntRect texCoords, bool collides, std::string colliderName, sf::Vector2f collOffsetFraction, sf::Vector2f collSizeFraction)
{
    init(game, chunk, localPosition, type, texCoords, collides, colliderName, collOffsetFraction, collSizeFraction);
}

void Tile::init(Game* game, Chunk* chunk, sf::Vector2i localPosition, TileType type, sf::IntRect texCoords, bool collides, std::string colliderName, sf::Vector2f collOffsetFraction, sf::Vector2f collSizeFraction)
{
    this->game = game;

    this->chunk = chunk;

    this->localPosition = localPosition;

    size = game->getSettings()->tile_size;

    this->type = type;

    myVerts.texCoords = texCoords;

    animation = nullptr;

    this->collides = collides;

    this->colliderName = colliderName;
    
    this->collOffsetFraction = collOffsetFraction;
    
    this->collSizeFraction = collSizeFraction;

    this->chunkVertices = chunk->getVertices();
}

sf::FloatRect Tile::getCollRect()
{
    if (collides)
    {
        if (chunk)
        {
            sf::Vector2f collSize = {size * collSizeFraction.x, size * collSizeFraction.y};
            
            sf::FloatRect tileRect = chunk->getTileRect(localPosition);
    
            return sf::FloatRect({tileRect.position.x + size / 2.f - collSize.x / 2.f, tileRect.position.y + size / 2.f - collSize.y / 2.f}, collSize);
        }
        else
        {
            std::cout << "ERROR tile could not get collision rect because it doesn't have a chunk ptr.\n";

            return sf::FloatRect({0, 0}, {-1, -1});
        }
    }
    else
    {
        return sf::FloatRect({0, 0}, {-1, -1});
    }
}

void Tile::update()
{
    if (animation)
    {
        if (animation->animation.name != "")
        {
            sf::IntRect newTexCoords = animation->animation.frames[animation->animation.index];
    
            if (myVerts.texCoords != newTexCoords)
            {
                myVerts.texCoords = newTexCoords;
                chunk->createTileVerts(localPosition);
            }
        }
    }
}