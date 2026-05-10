#include "mine_action.hpp"
#include "../../core/game.hpp"
#include <SFML/System/Vector2.hpp>

MineAction::MineAction(float mineSpeed, std::string name, float rangeMultiplier) : Action(name, rangeMultiplier, 0.f, 0.f, true), mineSpeed(mineSpeed) {}

void MineAction::start(Game* game)
{
    sf::Vector2i chunkPos = worldToChunkPosition(game, clickPosition);

    if (Chunk* chunk = game->getScene()->getChunkLayer()->getChunk(chunkPos))
    {
        sf::Vector2i tilePos = worldToTilePosition(game, clickPosition);
        tilePos = {tilePos.x % game->getSettings()->chunk_size, tilePos.y % game->getSettings()->chunk_size};
        
        std::vector<std::unique_ptr<TileTag>>* tags = &chunk->getTile(tilePos.x, tilePos.y)->tags;

        for (auto& t : *tags)
        {
            if (auto m = dynamic_cast<MineableTag*>(t.get()))
            {
                timeToComplete = m->durability / mineSpeed;
                cooldown = m->durability / mineSpeed;
            }
        }
    }
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
    // TEMP


    if (worldToTilePosition(game, game->getInput()->getMouseWorldPos()) != worldToTilePosition(game, clickPosition))
    {
        timeProgress = 0.f;
        clickPosition = game->getInput()->getMouseWorldPos();
        start(game);
    }
    
    return true;
}

void MineAction::completeAction(Entity* actor, sf::Vector2f position)
{
    sf::Vector2i chunkPos = worldToChunkPosition(actor->game, clickPosition);

    if (Chunk* chunk = actor->game->getScene()->getChunkLayer()->getChunk(chunkPos))
    {
        sf::Vector2i tilePos = worldToTilePosition(actor->game, clickPosition);
        tilePos = {tilePos.x % actor->game->getSettings()->chunk_size, tilePos.y % actor->game->getSettings()->chunk_size};
        
        TextureAtlas* atlas = actor->game->getAssetManager()->getTextureAtlas("tiles_better");

        Tile newTile = Tile(actor->game, chunk, tilePos, TileType::AIR, atlas->getItemTexCoords("air"), chunk->getHighestNonAirZ(tilePos.x, tilePos.y));

        if (newTile.z != -1) chunk->setTile(std::move(newTile));
    }
}