#include "mine_action.hpp"
#include "../../core/game.hpp"
#include <SFML/System/Vector2.hpp>
#include "../../ui/image_ui_element.hpp"

MineAction::MineAction(Game* game, float mineSpeed, std::string name, float rangeMultiplier) : Action(game, name, rangeMultiplier, 0.f, 0.f, true, true), mineSpeed(mineSpeed), mineZ(-1), m_indicator(nullptr) {}

bool MineAction::start()
{
    timeProgress = 0.f;
    timeToComplete = 0.f;;
    cooldown = 0.f;
    cooldownProgress = 0;

    startTile = game->getInputManager()->cursor->getSelectedTile();

    if (startTile)
    {
        std::vector<std::unique_ptr<TileTag>>* tags = &startTile->tags;

        for (auto& t : *tags)
        {
            auto m = dynamic_cast<MineableTag*>(t.get());
            if (!m) continue;

            timeToComplete = m->durability / mineSpeed;
            cooldown = m->durability / mineSpeed;
            
            // REFACTOR
            // UIElement* indicator = game->getScene()->getUILayer()->createElement(std::make_unique<UIElement>(game, "__MineAction indicator", UIPosition(cursorTileWorldPosTileCenter, UIOrigin::TOP_LEFT, UIAnchor::TOP_LEFT, true)));
            // indicator->addComponent<ImageComponent>(
            //     game,
            //     indicator,
            //     UIPosition({0, 0}, UIOrigin::CENTER),
            //     "image",
            //     0,
            //     game->getAssetManager()->getTexture("mine_progress_orb", "texture_atlases/ui/actions/MineAction/"),
            //     sf::Vector2f(tileSize, tileSize),
            //     false,
            //     game->getAssetManager()->getAnimation("mine_progress_orb", "animations/ui/actions/MineAction/"),
            //     nullptr,
            //     false,
            //     sf::IntRect({0, 0}, {0, 0}),
            //     1/timeToComplete,
            //     false
            // );
            // indicator->updateVisuals();
                
            sf::Vector2f cursorWorldPos(game->getInputManager()->cursor->getGameCursorWorldPosition());
            sf::Vector2i cursorTilePos = worldToTilePosition(game, cursorWorldPos, false, false);
            sf::Vector2f cursorTileWorldPos(tileToWorldPosition(game, cursorTilePos, false));
            float tileSize = game->getSettings()->tile_size;
            sf::Vector2f cursorTileWorldPosTileCenter(cursorTileWorldPos + sf::Vector2f(tileSize / 2, tileSize / 2));
                
            m_indicator = game->getScene()->getUILayer()->addElement<ImageUIElement>("position: " + std::to_string(cursorTileWorldPosTileCenter.x) + ", " + std::to_string(cursorTileWorldPosTileCenter.y) + ", 4, 0, 1; texture: images/mining.png; size: " + std::to_string(tileSize) + ", " + std::to_string(tileSize) + "; color: 255, 255, 255, 200");

            active = true;
            return true;
        }
    }

    active = false;
    return false;
}

bool MineAction::update(float dt)
{   
    timeProgress += dt;
    
    // game->getScene()->getUILayer()->getElement("__MineAction indicator")->updateVisuals();

    if (game->getInputManager()->cursor->getSelectedTile() != startTile)
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
        chunk->destroyTile(startTile->localPosition, &chunk->chunkLayer->tManager.tileTemplates["air"]);
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

    if (m_indicator) game->getScene()->getUILayer()->deleteElement(m_indicator->getID());    
}

std::unique_ptr<Action> MineAction::clone()
{
    return std::make_unique<MineAction>(*this);
}
