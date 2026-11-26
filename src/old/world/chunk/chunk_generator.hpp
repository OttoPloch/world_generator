#pragma once

#include <unordered_map>

#include "../../core/common.hpp"
#include "../../utils/FastNoiseLite.h"
#include "chunk.hpp"
#include "../tile/tile.hpp"

class Game;
class TextureAtlas;

class ChunkGenerator
{
public:
    ChunkGenerator();
    
    ChunkGenerator(Game* game, std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash>* chunks);

    void init(Game* game, std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash>* chunks);

    void generate(sf::Vector2i chunkPosition);
private:
    void generateChunk(sf::Vector2i chunkPosition);

    void generateDecorations(Chunk* chunk);

    void createDecoration(sf::Vector2f& chunkWorldPositon, float& scale, float& positionRounding, sf::Vector2f& decorationLocalBottom, sf::FloatRect& decorationTexCoords, sf::Vector2f& decorationSize, sf::Vector2f& decorationGlobalTopleft);

    bool decorationTileTypeCheck(Chunk* chunk, std::vector<TileType> validTypes, sf::Vector2f decorationGlobalTopleft, sf::Vector2f decorationSize);

    Game* game;

    std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash>* chunks;

    FastNoiseLite noise;

    TextureAtlas* decorationAtlas;
};