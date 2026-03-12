#pragma once

#include "common.hpp"
#include "entity.hpp"
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

    void loadNearbyChunks();

    bool loadChunk(sf::Vector2i chunkPosition);
    
    bool unloadChunk(sf::Vector2i chunkPosition);

    Chunk* getChunk(sf::Vector2i chunkPosition);

    // can get neighbors from other chunks
    std::array<Tile*, 8> getTileNeighbors(sf::Vector2i chunkPos, int column, int row);

    void tick();
    
    void update();

    void draw(bool debug = false);
private:
    Game* game;

    Window* window;
    
    int chunkSize;
    float tileSize;
    float chunkLength;

    std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash> chunks;

    ChunkGenerator chunkGenerator;

    // for updating the chunk pos display text. Can delete if no longer used
    sf::Vector2i lastChunkPos;
};