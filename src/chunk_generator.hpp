#pragma once

#include "common.hpp"
#include "chunk.hpp"
#include "tile.hpp"

#include <unordered_map>

class Game;

class ChunkGenerator
{
public:
    ChunkGenerator();
    
    ChunkGenerator(Game* game, std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash>* chunks);

    void init(Game* game, std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash>* chunks);

    void generate(sf::Vector2i chunkPosition, int genMode = 0);
private:
    Game* game;

    std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash>* chunks;
};