#pragma once

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

#include "../background_object.hpp"
#include "../tile/tile.hpp"
#include "chunk_state.hpp"

class Game;
class Window;

class Chunk
{
public:
    Chunk();

    Chunk(Game* game, sf::Vector2i chunkPosition, std::vector<std::vector<Tile>> tiles);

    void createTileVerts(int index, int z);

    void createTileVerts(sf::Vector2i tilePosition, int z);

    // values will wrap, so position
    // of -1 gets the opposite side and
    // 17 (in a chunk of size 16)
    // will get the left. Z also wraps.
    Tile* getTile(int column, int row, int z = 0);

    std::vector<std::vector<std::unique_ptr<Tile>>>* getTiles();

    sf::FloatRect getTileRect(sf::Vector2i tileLocalPosition, int z = 0, bool returnCenterPos = true);

    std::vector<sf::Vertex>* getVertices();

    sf::Vector2i getChunkPosition();

    void tick();

    void update(float dt);

    void draw(bool debug = false, int debugLayerView = 0);

    ChunkState state;

    std::vector<BackgroundObject> bgObjects;

    std::vector<Tile*> tilesWithColliders;
private:
    Game* game;

    Window* window;

    sf::Vector2i chunkPosition;

    int chunkSize;
    float tileSize;
    
    sf::Vector2f worldPosition;
    
    // tiles have a z for their height value, this
    // 2D vector separates them by that value.
    std::vector<std::vector<std::unique_ptr<Tile>>> tiles;

    std::vector<sf::Vertex> tileVertices;
    std::vector<std::vector<sf::Vertex>> tileDebugVertices;

    sf::RenderStates tileStates;
};