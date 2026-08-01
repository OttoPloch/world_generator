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

    startTile = game->getInput()->cursor->getSelectedTile();

    if (startTile)
    {
        std::vector<std::unique_ptr<TileTag>>* tags = &startTile->tags;

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

    
    if (game->getInput()->cursor->getSelectedTile() != startTile)
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
        chunk->setTile(startTile->localPosition, &chunk->chunkLayer->tManager.tileTemplates["air"]);
    }

    active = false;
}