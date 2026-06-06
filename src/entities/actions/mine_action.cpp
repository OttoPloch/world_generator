#include "mine_action.hpp"
#include "../../core/game.hpp"
#include <SFML/System/Vector2.hpp>

MineAction::MineAction(float mineSpeed, std::string name, float rangeMultiplier) : Action(name, rangeMultiplier, 0.f, 0.f, true, true), mineSpeed(mineSpeed), mineZ(-1) {}

bool MineAction::start(Game* game)
{
    timeProgress = 0.f;
    timeToComplete = 0.f;
    cooldownProgress = 0.f;
    cooldown = 0.f;

    sf::Vector2i chunkPos = worldToChunkPosition(game, startPosition);

    if (Chunk* chunk = game->getScene()->getChunkLayer()->getChunk(chunkPos))
    {
        sf::Vector2i tilePos = worldToTilePosition(game, startPosition);
        tilePos = {tilePos.x % game->getSettings()->chunk_size, tilePos.y % game->getSettings()->chunk_size};
        
        bool getHighestNonAir = false;
        if (mineZ == -1) getHighestNonAir = true;

        Tile* tile = chunk->getTile(tilePos.x, tilePos.y, getHighestNonAir, mineZ);
        
        if (tile)
        {
            std::vector<std::unique_ptr<TileTag>>* tags = &tile->tags;

            for (auto& t : *tags)
            {
                if (auto m = dynamic_cast<MineableTag*>(t.get()))
                {
                    timeToComplete = m->durability / mineSpeed;
                    cooldown = m->durability / mineSpeed;

                    return true;
                }
            }
        }
    }

    active = false;
    return false;
}

bool MineAction::update(float dt, Game* game)
{   
    timeProgress += dt;


    // TEMP but cool
    int progress = std::trunc(timeProgress * 10);
    int left = std::trunc(timeToComplete * 10) - progress;

    for (int j = 0; j < progress; j++) std::cout << "+";
    for (int j = 0; j < left; j++) std::cout << ".";
    std::cout << '\n';


    // TODO, replace the ui collision check here if it is too expensive, or abstract that check to the Action struct so it doesn't need to be implemented in every type of action.
    if (worldToTilePosition(game, game->getInput()->getMouseCoords()) != worldToTilePosition(game, startPosition) || game->getScene()->getUILayer()->checkUICollision())
    {
        return false;
    }
    
    return true;
}

void MineAction::completeAction(Entity* actor, sf::Vector2f position)
{
    sf::Vector2i chunkPos = worldToChunkPosition(actor->game, startPosition);

    if (Chunk* chunk = actor->game->getScene()->getChunkLayer()->getChunk(chunkPos))
    {
        sf::Vector2i tilePos = worldToTilePosition(actor->game, startPosition);
        tilePos = {tilePos.x % actor->game->getSettings()->chunk_size, tilePos.y % actor->game->getSettings()->chunk_size};

        chunk->setTile(tilePos.x, tilePos.y, &chunk->chunkLayer->tManager.tileTemplates["air"]);
    }

    active = false;
}