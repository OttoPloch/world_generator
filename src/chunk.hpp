#pragma once

#include "common.hpp"
#include "tile.hpp"

#include <vector>

class Game;
class Window;

class Chunk
{
public:
    Chunk();

    Chunk(Game* game, sf::Vector2i chunkPosition);

    void init(Game* game, sf::Vector2i chunkPosition);

    void createTiles();

    void createTileVerts(int index);

    std::vector<Tile>* getTiles();

    sf::Vector2i getChunkPosition();

    void tick();

    void draw();
private:
    Game* game;

    Window* window;

    sf::Vector2i chunkPosition;

    int chunkLength;

    float tileSize;
    
    sf::Vector2f worldPosition;
    
    std::vector<Tile> tiles;

    std::vector<sf::Vertex> tileVertices;
};