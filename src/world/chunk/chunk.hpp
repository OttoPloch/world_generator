#pragma once

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

#include "../background_object.hpp"
#include "../tile/tile.hpp"
#include "chunk_state.hpp"
#include "../tile/tags/mineable_tag.hpp"
#include "../tile/tile_template.hpp"

class Game;
class Window;
class ChunkLayer;

class Chunk
{
public:
    Chunk();

    Chunk(Game* game, ChunkLayer* chunkLayer, sf::Vector2i chunkPosition, std::vector<std::vector<TileTemplate>> tileData);

    void createTileVerts(int index, int z);

    void createTileVerts(sf::Vector2i tilePosition, int z);

    // values will wrap, so position
    // of -1 gets the opposite side and
    // 17 (in a chunk of size 16)
    // will get the left. Z also wraps.
    Tile* getTile(int column, int row, bool getHighestNonAir = true, int z = 0);

    void setTile(int column, int row, TileTemplate* t, bool setHighestNonAir = true, int z = 0);

    void setTile(int index, TileTemplate* t, bool setHighestNonAir = true, int z = 0);

    std::vector<std::vector<std::unique_ptr<Tile>>>* getTiles();

    sf::FloatRect getTileRect(sf::Vector2i tileLocalPosition, int z = 0, bool returnCenterPos = true);

    std::vector<sf::Vertex>* getVertices();

    // return value will be in a valid range for z,
    // unless it returns -1 which indicates that
    // all tiles at that position are air or there
    // are no tiles there.
    int getHighestNonAirZ(int& column, int& row, bool alsoWrapPosition = true);

    void wrapPosition(int& column, int& row);

    void wrapPosition(int& column, int& row, int& z);

    sf::Vector2i getChunkPosition();

    void tick();

    void update(float dt);

    void draw(bool debug = false, int debugLayerView = 0);

    ChunkState state;

    std::vector<BackgroundObject> bgObjects;

    std::vector<Tile*> tilesWithColliders;
    
    ChunkLayer* chunkLayer;
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