#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <unordered_map>

#include "../../core/common.hpp"
#include "../../entities/entity.hpp"
#include "../../core/window.hpp"
#include "chunk.hpp"
#include "chunk_generator.hpp"
#include "../tile/tile_template_manager.hpp"

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

    std::array<Chunk*, 9> getNearbyChunks(sf::Vector2f position);

    std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash>* getAllLoadedChunks();

    Tile* getTileAtPosition(sf::Vector2f position, bool activeChunksOnly);

    void tick();
    
    void loadUpdate();

    void update(float dt);

    void draw(bool debug = false, int debugLayerView = 0);

    TileTemplateManager tManager;
private:
    sf::Vector2i getCurrChunkPos();

    Game* game;

    Window* window;
    
    int chunkSize;
    float tileSize;
    float chunkLength;

    ChunkGenerator chunkGenerator;
    
    std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash> chunks;

    // background objects
    std::vector<sf::Vertex> bgObjectsVertices;
    sf::RenderStates bgObjectStates;
    
    sf::Vector2i lastChunkPos;
};