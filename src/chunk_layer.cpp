#include "chunk_layer.hpp"
#include "chunk.hpp"
#include "chunk_state.hpp"
#include "entity_layer.hpp"
#include "game.hpp"
#include "utils.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <array>
#include <string>

ChunkLayer::ChunkLayer() : chunks(0) {}

ChunkLayer::ChunkLayer(Game* game) : chunks(0)
{
    init(game);
}

void ChunkLayer::init(Game* game)
{
    this->game = game;

    this->window = game->getWindow();

    chunkGenerator.init(game, &chunks);

    lastChunkPos = {99999, 99999};

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

std::vector<std::vector<Tile>*> ChunkLayer::getSurroundingTiles(sf::Vector2f position)
{
    sf::Vector2i convertedPosition = worldToChunkPosition(game, position);

    std::array<sf::Vector2i, 9> chunksToSearch = {
        sf::Vector2i(convertedPosition.x - 1, convertedPosition.y - 1),
        sf::Vector2i(convertedPosition.x, convertedPosition.y - 1),
        sf::Vector2i(convertedPosition.x + 1, convertedPosition.y - 1),
        sf::Vector2i(convertedPosition.x - 1, convertedPosition.y),
        convertedPosition,
        sf::Vector2i(convertedPosition.x + 1, convertedPosition.y),
        sf::Vector2i(convertedPosition.x - 1, convertedPosition.y + 1),
        sf::Vector2i(convertedPosition.x, convertedPosition.y + 1),
        sf::Vector2i(convertedPosition.x + 1, convertedPosition.y + 1)
    };

    std::vector<std::vector<Tile>*> surroundingTiles;

    for (int i = 0; i < 9; i++)
    {
        if (chunks.find(chunksToSearch[i]) != chunks.end())
        {
            surroundingTiles.push_back(chunks[chunksToSearch[i]]->getTiles());
        }
    }

    return surroundingTiles;
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
    for (auto& i : chunks)
    {
        if (i.second->state == ChunkState::ACTIVE)
        {
            i.second->draw(debug);
        }

        if (debug)
        {
            float chunkLength = toFloat(game->getSettings()->getSetting("chunk_size").valueInt) * game->getSettings()->getSetting("tile_size").valueFloat;
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