#include "chunk_layer.hpp"
#include "chunk.hpp"
#include "entity_layer.hpp"
#include "game.hpp"
#include "utils.hpp"
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

    lastChunkPos = {999, 999};
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

void ChunkLayer::update()
{
    int renderDist = game->getSettings()->getSetting("chunk_render_distance").valueInt;

    sf::Vector2i centerChunk = worldToChunkPosition(game, window->getWindow().getView().getCenter());

    for (int y = -renderDist; y <= renderDist; y++)
    {
        for (int x = -renderDist; x <= renderDist; x++)
        {
            loadChunk({centerChunk.x + x, centerChunk.y + y});
        }
    }

    std::vector<sf::Vector2i> chunksToDelete;
    for (auto& i : chunks)
    {
        if (abs(centerChunk.x - i.second->getChunkPosition().x) > renderDist || abs(centerChunk.y - i.second->getChunkPosition().y) > renderDist)
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

void ChunkLayer::tick()
{
    for (auto& i : chunks)
    {
        i.second->tick();
    }

    sf::Vector2i currChunkPos = worldToChunkPosition(game, game->getScene()->getCamera()->getCenter());

    if (currChunkPos != lastChunkPos)
    {
        game->getScene()->getUILayer()->getElement("chunk pos display")->getAsText()->setValue(std::to_string(currChunkPos.x) + ", " + std::to_string(currChunkPos.y));

        lastChunkPos = currChunkPos;
    }
}

void ChunkLayer::draw(bool debug)
{
    for (auto& chunk : chunks)
    {
        chunk.second->draw(debug);

        if (debug)
        {
            sf::RectangleShape rect({
                toFloat(game->getSettings()->getSetting("chunk_size").valueInt) * game->getSettings()->getSetting("tile_size").valueFloat,
                toFloat(game->getSettings()->getSetting("chunk_size").valueInt) * game->getSettings()->getSetting("tile_size").valueFloat
            });

            rect.setPosition(chunkToWorldPosition(game, chunk.second->getChunkPosition()));
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineThickness(5.f);

            window->draw(rect);
        }
    }
}