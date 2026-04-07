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

    Chunk(Game* game, sf::Vector2i chunkPosition, std::vector<Tile> tiles);

    void init(Game* game, sf::Vector2i chunkPosition, std::vector<Tile> tiles);

    void createTileVerts(int index);

    void createTileVerts(sf::Vector2i tilePosition);

    // values will wrap, so -1
    // gets the opposite side and
    // 17 (in a chunk of size 16)
    // will get the left
    Tile* getTile(int column, int row);

    std::vector<std::unique_ptr<Tile>>* getTiles();

    sf::FloatRect getTileRect(sf::Vector2i tileLocalPosition);

    std::vector<sf::Vertex>* getVertices();

    sf::Vector2i getChunkPosition();

    void tick();

    void update(float dt);

    void draw(bool debug = false);

    ChunkState state;

    std::vector<BackgroundObject> bgObjects;
private:
    Game* game;

    Window* window;

    sf::Vector2i chunkPosition;

    int chunkSize;
    float tileSize;
    
    sf::Vector2f worldPosition;
    
    std::vector<std::unique_ptr<Tile>> tiles;

    std::vector<sf::Vertex> tileVertices;
    std::vector<sf::Vertex> tileDebugVertices;

    sf::RenderStates tileStates;
};