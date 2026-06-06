#pragma once

#include <unordered_map>

#include "../../core/common.hpp"
#include "../../utils/FastNoiseLite.h"
#include "chunk.hpp"
#include "../tile/tile.hpp"

class Game;

class ChunkGenerator
{
public:
    ChunkGenerator();
    
    ChunkGenerator(Game* game, std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash>* chunks);

    void init(Game* game, std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash>* chunks);

    void generate(sf::Vector2i chunkPosition);
private:
    Game* game;

    std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash>* chunks;

    FastNoiseLite noise;
};