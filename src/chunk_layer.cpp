#include "chunk_layer.hpp"
#include "chunk.hpp"
#include "chunk_state.hpp"
#include "entity_layer.hpp"
#include "game.hpp"
#include "utils.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>

ChunkLayer::ChunkLayer() : chunks(0) {}

ChunkLayer::ChunkLayer(Game* game) : chunks(0)
{
    init(game);
}

void ChunkLayer::init(Game* game)
{
    this->game = game;

    this->window = game->getWindow();

    chunkSize = game->getSettings()->getSetting("chunk_size").valueInt;
    tileSize = game->getSettings()->getSetting("tile_size").valueFloat;
    chunkLength = toFloat(chunkSize) * tileSize;

    chunkGenerator.init(game, &chunks);

    lastChunkPos = {INT32_MAX, INT32_MAX};

    loadNearbyChunks();
}

void ChunkLayer::loadNearbyChunks()
{
    int renderDist = game->getSettings()->getSetting("chunk_render_distance").valueInt;
    int loadDist = game->getSettings()->getSetting("chunk_load_distance").valueInt;
 
    sf::Vector2i currChunkPos = worldToChunkPosition(game, game->getScene()->getCamera()->getCenter());

    if (currChunkPos != lastChunkPos)
    {
        for (int y = -loadDist; y <= loadDist; y++)
        {
            for (int x = -loadDist; x <= loadDist; x++)
            {
                loadChunk({currChunkPos.x + x, currChunkPos.y + y});

                if (y >= -renderDist && y <= renderDist && x >= -renderDist && x <= renderDist)
                {
                    chunks[{currChunkPos.x + x, currChunkPos.y + y}]->state = ChunkState::ACTIVE;
                }
                else
                {
                    chunks[{currChunkPos.x + x, currChunkPos.y + y}]->state = ChunkState::ASLEEP;
                }
            }
        }
    }
}

bool ChunkLayer::loadChunk(sf::Vector2i chunkPosition)
{   
    if (chunks.find(chunkPosition) == chunks.end())
    {
        chunkGenerator.generate(chunkPosition, 2);

        return true;
    }

    return false;
}

bool ChunkLayer::unloadChunk(sf::Vector2i chunkPosition)
{
    if (chunks.find(chunkPosition) != chunks.end())
    {
        chunks.erase(chunkPosition);

        game->getScene()->getEntityLayer()->removeAllEntitiesInChunk(chunkPosition.x, chunkPosition.y);

        return true;
    }

    return false;
}

Chunk* ChunkLayer::getChunk(sf::Vector2i chunkPosition)
{
    if (chunks.find(chunkPosition) != chunks.end())
    {
        return chunks[chunkPosition].get();
    }

    return nullptr;
}

std::array<Tile*, 8> ChunkLayer::getTileNeighbors(sf::Vector2i chunkPos, int column, int row)
{
    std::array<Tile*, 8> neighbors;

    Chunk* chunk = chunks[chunkPos].get();

    if (column != 0 && column != chunkSize - 1 && row != 0 && row != chunkSize - 1)
    {
        neighbors[0] = chunk->getTile(column - 1, row - 1);
        neighbors[1] = chunk->getTile(column, row - 1);
        neighbors[2] = chunk->getTile(column + 1, row - 1);
        neighbors[3] = chunk->getTile(column - 1, row);
        neighbors[4] = chunk->getTile(column + 1, row);
        neighbors[5] = chunk->getTile(column - 1, row + 1);
        neighbors[6] = chunk->getTile(column, row + 1);
        neighbors[7] = chunk->getTile(column + 1, row + 1);
    }
    else
    {
        if (column != 0)
        {
            (row != 0) ? neighbors[0] = chunk->getTile(column - 1, row - 1) : neighbors[0] = nullptr;
            neighbors[3] = chunk->getTile(column - 1, row);
            (row != chunkSize - 1) ? neighbors[5] = chunk->getTile(column - 1, row + 1) : neighbors[5] = nullptr;
        }
        else
        {
            Chunk* leftChunk = chunks[{chunkPos.x - 1, chunkPos.y}].get();

            if (leftChunk)
            {
                (row != 0) ? neighbors[0] = leftChunk->getTile(-1, row - 1) : neighbors[0] = nullptr;
                neighbors[3] = leftChunk->getTile(-1, row);
                (row != chunkSize - 1) ? neighbors[5] = leftChunk->getTile(-1, row + 1) : neighbors[5] = nullptr;
            }
            else
            {
                neighbors[0] = nullptr;
                neighbors[3] = nullptr;
                neighbors[5] = nullptr;
            }
        }

        if (column != chunkSize - 1)
        {
            (row != 0) ? neighbors[2] = chunk->getTile(column + 1, row - 1) : neighbors[2] = nullptr;
            neighbors[4] = chunk->getTile(column + 1, row);
            (row != chunkSize - 1) ? neighbors[7] = chunk->getTile(column + 1, row + 1) : neighbors[7] = nullptr;
        }
        else
        {
            Chunk* rightChunk = chunks[{chunkPos.x + 1, chunkPos.y}].get();

            if (rightChunk)
            {
                (row != 0) ? neighbors[2] = rightChunk->getTile(0, row - 1) : neighbors[2] = nullptr;
                neighbors[4] = rightChunk->getTile(0, row);
                (row != chunkSize - 1) ? neighbors[7] = rightChunk->getTile(0, row + 1) : neighbors[7] = nullptr;
            }
            else
            {
                neighbors[2] = nullptr;
                neighbors[4] = nullptr;
                neighbors[7] = nullptr;
            }
        }

        if (row != 0)
        {
            neighbors[1] = chunk->getTile(column, row - 1);
        }
        else
        {
            Chunk* topChunk = chunks[{chunkPos.x, chunkPos.y - 1}].get();

            if (topChunk)
            {
                neighbors[1] = topChunk->getTile(column, -1);
            }
            else
            {
                neighbors[1] = nullptr;
            }
        }

        if (row != chunkSize - 1)
        {
            neighbors[6] = chunk->getTile(column, row + 1);
        }
        else
        {
            Chunk* bottomChunk = chunks[{chunkPos.x, chunkPos.y + 1}].get();

            if (bottomChunk)
            {
                neighbors[6] = bottomChunk->getTile(column, 0);
            }
            else
            {
                neighbors[6] = nullptr;
            }
        }
    }

    return neighbors;
}

void ChunkLayer::tick()
{
    for (auto& i : chunks)
    {
        if (i.second->state == ChunkState::ACTIVE)
        {
            i.second->tick();
        }
    }

    sf::Vector2i currChunkPos = worldToChunkPosition(game, game->getScene()->getCamera()->getCenter());

    if (currChunkPos != lastChunkPos)
    {
        game->getScene()->getUILayer()->getElement("chunk pos display")->getAsText()->setValue(std::to_string(currChunkPos.x) + ", " + std::to_string(currChunkPos.y));

        lastChunkPos = currChunkPos;
    }
}

void ChunkLayer::update()
{
    int loadDist = game->getSettings()->getSetting("chunk_load_distance").valueInt;

    sf::Vector2i currChunkPos = worldToChunkPosition(game, game->getScene()->getCamera()->getCenter());

    loadNearbyChunks();

    std::vector<sf::Vector2i> chunksToDelete;
    for (auto& i : chunks)
    {
        if (abs(currChunkPos.x - i.second->getChunkPosition().x) > loadDist || abs(currChunkPos.y - i.second->getChunkPosition().y) > loadDist)
        {
            chunksToDelete.push_back(i.second->getChunkPosition());
        }
    }

    if (chunksToDelete.size() > 0)
    {
        for (int i = 0; i < chunksToDelete.size(); i++)
        {
            unloadChunk(chunksToDelete[i]);
        }
    }
}

void ChunkLayer::draw(bool debug)
{
    for (int layer = 0; layer < 2; layer++)
    {
        for (auto& i : chunks)
        {
            if (i.second->state == ChunkState::ACTIVE)
            {
                sf::Vector2f chunkTl = chunkToWorldPosition(game, i.second->getChunkPosition());
    
                if (isOnScreen(game, chunkTl, {chunkLength, chunkLength}))
                {
                    i.second->draw(layer, debug);
                }
            }
    
            if (debug)
            {
                sf::Vector2f chunkPos = chunkToWorldPosition(game, i.second->getChunkPosition());
    
                sf::Vertex tl;
                sf::Vertex tr;
                sf::Vertex bl;
                sf::Vertex br;
     
                tl.position = chunkPos;
                tr.position = {chunkPos.x + chunkLength, chunkPos.y};
                bl.position = {chunkPos.x, chunkPos.y + chunkLength};
                br.position = {chunkPos.x + chunkLength, chunkPos.y + chunkLength};
    
                std::vector<sf::Vertex> chunkOutline = {tl, tr, br, bl, tl};
    
                window->getWindow().draw(&chunkOutline[0], chunkOutline.size(), sf::PrimitiveType::LineStrip);
            }
        }
    }
}