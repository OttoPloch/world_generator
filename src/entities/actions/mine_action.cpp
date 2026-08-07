#include "mine_action.hpp"
#include "../../core/game.hpp"
#include <SFML/System/Vector2.hpp>
#include "../../ui/components/image_component.hpp"

MineAction::MineAction(Game* game, float mineSpeed, std::string name, float rangeMultiplier) : Action(game, name, rangeMultiplier, 0.f, 0.f, true, true), mineSpeed(mineSpeed), mineZ(-1) {}

bool MineAction::start()
{
    timeProgress = 0.f;
    timeToComplete = 0.f;;
    cooldown = 0.f;
    cooldownProgress = 0;

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

                // UIElement* cursorElement = game->getInput()->cursor->getCursorElement();
                // cursorElement->addComponent<ImageComponent>(
                //     game,
                //     cursorElement,
                //     UIPosition({0, 0}, UIOrigin::BOTTOM_RIGHT),
                //     "MineAction indicator",
                //     0,
                //     game->getAssetManager()->getTexture("mine_progress", "texture_atlases/ui/actions/MineAction/"),
                //     sf::Vector2f(25, 25),
                //     false,
                //     game->getAssetManager()->getAnimation("mine_progress", "animations/ui/actions/MineAction/"),
                //     nullptr,
                //     false,
                //     sf::IntRect({0, 0}, {0, 0}),
                //     1/timeToComplete
                // );
                sf::Vector2f cursorWorldPos = game->getInput()->cursor->getGameCursorCoords();
                sf::Vector2i cursorTilePos = worldToTilePosition(game, cursorWorldPos);
                sf::Vector2f cursorTileWorldPos = tileToWorldPosition(game, cursorTilePos);
                sf::Vector2f cursorTileWorldPosTileCenter = cursorTileWorldPos + sf::Vector2f(game->getSettings()->tile_size / 2, game->getSettings()->tile_size / 2);

                UIElement* indicator = game->getScene()->getUILayer()->createElement(std::make_unique<UIElement>(game, "__MineAction indicator", UIPosition(cursorTileWorldPosTileCenter, UIOrigin::CENTER, UIAnchor::TOP_LEFT, true)));
                indicator->addComponent<ImageComponent>(
                    game,
                    indicator,
                    UIPosition({0, 0}, UIOrigin::CENTER),
                    "image",
                    0,
                    game->getAssetManager()->getTexture("mine_progress", "texture_atlases/ui/actions/MineAction/"),
                    sf::Vector2f(10, 10),
                    false,
                    game->getAssetManager()->getAnimation("mine_progress", "animations/ui/actions/MineAction/"),
                    nullptr,
                    false,
                    sf::IntRect({0, 0}, {0, 0}),
                    1/timeToComplete
                );
                indicator->updateVisuals();

                active = true;
                return true;
            }
        }
    }

    active = false;
    return false;
}

bool MineAction::update(float dt)
{   
    timeProgress += dt;
    
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

    reset(true);
}

void MineAction::reset(bool restartCooldownProgress)
{
    timeToComplete = 0.f;
    timeProgress = 0.f;
    cooldown = 0.f;
    cooldownProgress = 0.f;
    active = false;

    // UIElement* cursorElement = game->getInput()->cursor->getCursorElement();
    // cursorElement->removeComponent<ImageComponent>("MineAction indicator");
    game->getScene()->getUILayer()->removeElement("__MineAction indicator");
}