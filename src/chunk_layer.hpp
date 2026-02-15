#pragma once

#include "common.hpp"
#include "window.hpp"
#include "chunk.hpp"
#include "chunk_generator.hpp"

#include <unordered_map>

class Game;

class ChunkLayer
{
public:
    ChunkLayer();

    ChunkLayer(Game* game);

    void init(Game* game);

    bool loadChunk(sf::Vector2i chunkPosition);
    
    bool unloadChunk(sf::Vector2i chunkPosition);
    
    std::vector<std::vector<Tile>*> getSurroundingTiles(sf::Vector2f position);

    void tick();

    void draw();
private:
    Game* game;

    Window* window;
    
    std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash> chunks;

    ChunkGenerator chunkGenerator;
};