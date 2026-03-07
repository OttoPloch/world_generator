#pragma once

#include "common.hpp"
#include "vertex_group.hpp"
#include "tile_types.hpp"

class Game;
class Chunk;

class Tile
{
public:
    Tile();

    // this is only used in ChunkGenerator to set the values of the tile before they are copied into the final Tile object in the Chunk.
    Tile(TileType type, sf::FloatRect texCoords, bool collides = false, std::string colliderName = "none", sf::Vector2f collOffsetFraction = {0.f, 0.f}, sf::Vector2f collSizeFraction = {1.f, 1.f});
    
    Tile(Game* game, Chunk* chunk, sf::Vector2i localPosition, TileType type, sf::FloatRect texCoords, bool collides = false, std::string colliderName = "none", sf::Vector2f collOffsetFraction = {0.f, 0.f}, sf::Vector2f collSizeFraction = {1.f, 1.f});

    void init(Game* game, Chunk* chunk, sf::Vector2i localPosition, TileType type, sf::FloatRect texCoords, bool collides = false, std::string colliderName = "none", sf::Vector2f collOffsetFraction = {0.f, 0.f}, sf::Vector2f collSizeFraction = {1.f, 1.f});

    sf::FloatRect getCollRect();

    Chunk* chunk;

    sf::Vector2i localPosition;

    float size;

    TileType type;

    bool collides;

    sf::Vector2f collOffsetFraction;

    sf::Vector2f collSizeFraction;

    std::string colliderName;

    VertexGroup myVerts;
private:
    Game* game;

    std::vector<sf::Vertex>* chunkVertices;
};