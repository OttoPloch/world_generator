#pragma once

#include "common.hpp"
#include "window.hpp"
#include "chunk.hpp"

#include <unordered_map>

class Game;

struct Vector2iHash
{
    std::size_t operator()(const sf::Vector2i& v) const noexcept
    {
        // Simple hash combine
        std::size_t h1 = std::hash<int>()(v.x);
        std::size_t h2 = std::hash<int>()(v.y);
        return h1 ^ (h2 << 1);
    }
};

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
    
    std::unordered_map<sf::Vector2i, Chunk, Vector2iHash> chunks;
};