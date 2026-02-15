#pragma once

#include "tile.hpp"

#include <vector>

class Game;
class Window;

class Chunk
{
public:
    Chunk();

    Chunk(Game* game, sf::Vector2i chunkPosition, std::vector<Tile> tiles);

    void init(Game* game, sf::Vector2i chunkPosition, std::vector<Tile> tiles);

    void createTileVerts(int index);

    std::vector<Tile>* getTiles();

    sf::FloatRect getTileRect(sf::Vector2i tileLocalPosition);

    sf::Vector2i getChunkPosition();

    void tick();

    void draw(bool debug = false);
private:
    Game* game;

    Window* window;

    sf::Vector2i chunkPosition;

    int chunkSize;

    float tileSize;
    
    sf::Vector2f worldPosition;
    
    std::vector<Tile> tiles;

    std::vector<sf::Vertex> tileVertices;
};