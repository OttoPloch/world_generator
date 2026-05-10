#pragma once

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

#include "../background_object.hpp"
#include "../tile/tile.hpp"
#include "chunk_state.hpp"
#include "../tile/tags/mineable_tag.hpp"

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
    Tile* getTile(int column, int row, int z = 0, bool getHighestNonAir = true);

    void setTile(Tile newTile, bool setHighestNonAir = true);

    std::vector<std::vector<std::unique_ptr<Tile>>>* getTiles();

    sf::FloatRect getTileRect(sf::Vector2i tileLocalPosition, int z = 0, bool returnCenterPos = true);

    std::vector<sf::Vertex>* getVertices();

    // if wrapValues is true, column and row arguments can be out
    // of bounds, and will wrap around. If it is false, you must
    // provide valid coordinates. The z return value will always
    // be in a valid range, not a negative UNLESS no non-air tile
    // is found, then it will return -1 to indicate a failure.
    int getHighestNonAirZ(int column, int row, bool wrapValues = true);

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