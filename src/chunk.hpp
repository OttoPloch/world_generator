#pragma once

#include "tile.hpp"
#include "chunk_state.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

class Game;
class Window;

class Chunk
{
public:
    Chunk();

    Chunk(Game* game, sf::Vector2i chunkPosition, std::vector<Tile> tiles);

    void init(Game* game, sf::Vector2i chunkPosition, std::vector<Tile> tiles);

    void createTileVerts(int index);

    void giveDecorationVerts(std::unique_ptr<std::vector<sf::Vertex>> vertices, sf::Texture* texture);

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

    void draw(int layer, bool debug = false);

    ChunkState state;
private:
    Game* game;

    Window* window;

    sf::Vector2i chunkPosition;

    int chunkSize;
    float tileSize;
    
    sf::Vector2f worldPosition;
    
    std::vector<std::unique_ptr<Tile>> tiles;

    std::vector<sf::Vertex> tileVertices;

    std::vector<sf::Vertex> debugVertices;

    std::unique_ptr<std::vector<sf::Vertex>> decorationVertices;

    sf::RenderStates tileStates;
    sf::RenderStates decorationStates;
};