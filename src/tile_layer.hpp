#pragma once

#include "common.hpp"
#include "window.hpp"
#include "tile.hpp"

#include <vector>

struct vertexRange
{
    size_t offset;
    size_t size;
};

class Game;

class TileLayer
{
public:
    TileLayer();

    TileLayer(Game* game);

    void init(Game* game);

    std::vector<Tile>* getTiles();
    
    void tick();

    void draw();
private:
    Game* game;

    Window* window;
    
    std::vector<Tile> tiles;
    
    std::vector<sf::Vertex> vertices;
    
    std::vector<vertexRange> freeRanges;
};