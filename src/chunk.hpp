#pragma once

#include "tile.hpp"
#include "background_object.hpp"

#include <SFML/Graphics/RenderStates.hpp>
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

    void createBgObjectVerts(std::vector<BackgroundObject> bgObjects);

    // values will wrap, so -1
    // gets the opposite side and
    // 17 (in a chunk of size 16)
    // will get the left
    Tile* getTile(int column, int row);

    std::vector<Tile>* getTiles();

    sf::FloatRect getTileRect(sf::Vector2i tileLocalPosition);

    sf::Vector2i getChunkPosition();

    void tick();

    void draw(int layer, bool debug = false);
private:
    Game* game;

    Window* window;

    sf::Vector2i chunkPosition;

    int chunkSize;

    float tileSize;
    
    sf::Vector2f worldPosition;
    
    std::vector<Tile> tiles;

    std::vector<sf::Vertex> tileVertices;

    std::vector<sf::Vertex> bgObjectVertices;
    sf::RenderStates bgObjectStates;
};