#include "scene.hpp"
#include "game.hpp"
#include "input.hpp"
#include "../entities/states.hpp"
#include "../utils/utils.hpp"
#include "../entities/actions/action.hpp"
#include "../entities/actions/mine_action.hpp"
#include "../entities/actions/mine_action.hpp"
#include "../ui/components/button_component.hpp"
#include "../entities/components/components.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Mouse.hpp>
#include <memory>
#include <string>

Scene::Scene() {}

void Scene::init(Game* game)
{
    this->game = game;

    window = game->getWindow();

    assetManager = game->getAssetManager();
    
    entityLayer.init(game);

    camera.init(game, false, {0, 0}, toV2F(window->getSize()), entityLayer.player);

    uiLayer.init(game, &camera);

    chunkLayer.init(game);

    debugMode = false;
    debugLevel = 0;
    debugChunkLayerView = -1;
}

void Scene::tick()
{   
    // std::cout << worldChunkOrigin.x << ", " << worldChunkOrigin.y << '\n';

    camera.tick();

    chunkLayer.tick();
    
    entityLayer.tick();

    uiLayer.tick();

    // TEMP
    sf::Vector2i mouseChunkPos = worldToChunkPosition(game, game->getInput()->cursor->getGameCursorCoords());
    // uiLayer.getElement("mouse chunk pos display")->getAsText()->setValue(std::to_string(mouseChunkPos.x) + ", " + std::to_string(mouseChunkPos.y));
    auto element = uiLayer.getElement("__debug text display");
    if (element)
    {
        if (auto chunkPosText = element->getComponent<TextComponent>("mouse chunk pos text"))
        {
            chunkPosText->setText("Mouse Chunk Pos: " + std::to_string(mouseChunkPos.x) + ", " + std::to_string(mouseChunkPos.y));
        }
    
        std::string mouseTileType = "none";
        Tile* selectedTile = game->getInput()->cursor->getSelectedTile();

        if (selectedTile)
        {
            std::map<TileType, std::string> typesToStrings {
                {TileType::AIR, "air"},
                {TileType::WATER, "water"},
                {TileType::GRASS, "grass"},
                {TileType::STONE, "stone"},
                {TileType::LAVA, "lava"},
                {TileType::COBBLE, "cobble"},
                {TileType::PINK, "pink"},
            };
        
            mouseTileType = typesToStrings[selectedTile->type];
        }

        if (auto tileTypeText = element->getComponent<TextComponent>("mouse tile type text"))
        {
            tileTypeText->setText("Mouse Tile Type: " + mouseTileType);
        }

        if (auto entityText = element->getComponent<TextComponent>("mouse entity text"))
        {
            auto e = game->getInput()->cursor->getSelectedEntity();

            if (e) entityText->setText("Entity ID: " + std::to_string(e->ID));
            else entityText->setText("No Entity Selected");
        }

        if (auto worldOriginText = element->getComponent<TextComponent>("world origin text"))
        {
            worldOriginText->setText("World Chunk Origin: " + std::to_string(worldChunkOrigin.x) + ", " + std::to_string(worldChunkOrigin.y));
        }
    }
    /////
}

void Scene::update(float dt)
{
    debugClock.restart();

    camera.update(dt);
    updateBlame["UPDATE_CAMERA"] = debugClock.restart().asSeconds();

    chunkLayer.update(dt);
    updateBlame["UPDATE_CHUNK_LAYER"] = debugClock.restart().asSeconds();
    
    entityLayer.update(dt);
    updateBlame["UPDATE_ENTITY_LAYER"] = debugClock.restart().asSeconds();

    if (debugMode && debugLevel == 1) printBlameStats(updateBlame, "SCENE_UPDATE");
}

void Scene::UIUpdate(float dt)
{
    if (auto e = uiLayer.getElement("speed buttons"))
    {
        if (auto slower = e->getComponent<ButtonComponent>("slower button"))
        {
            if (slower->justPressed())
            {
                if (entityLayer.player)
                {
                    if (auto c = entityLayer.player->getComponent<MovementComponent>())
                    {
                        c->stats.speed -= 2;

                        if (auto t = e->getComponent<TextComponent>("speed display"))
                        {
                            t->setText(std::to_string(toInt(c->stats.speed)));
                        }
                    }
                }
            }
        }
        
        if (auto faster = e->getComponent<ButtonComponent>("faster button"))
        {
            if (faster->justPressed())
            {
                if (entityLayer.player)
                {
                    if (auto c = entityLayer.player->getComponent<MovementComponent>())
                    {
                        c->stats.speed += 2;

                        if (auto t = e->getComponent<TextComponent>("speed display"))
                        {
                            t->setText(std::to_string(toInt(c->stats.speed)));
                        }
                    }
                }
            }
        }
    }

    // if (uiLayer.getElement("faster button")->getAsButton()->getActive())
    // {
    //     if (auto e = entityLayer.getEntity(0)->getComponent<MovementComponent>())
    //     {
    //         e->stats.speed += 2;
            
    //         uiLayer.getElement("speed display")->getAsText()->setValue(std::to_string(toInt(e->stats.speed)));
    //     }        
    // }
    
    // if (uiLayer.getElement("slower button")->getAsButton()->getActive())
    // {
    //     if (auto e = entityLayer.getEntity(0)->getComponent<MovementComponent>())
    //     {
    //         e->stats.speed -= 2;

    //         uiLayer.getElement("speed display")->getAsText()->setValue(std::to_string(toInt(e->stats.speed)));
    //     }
    // }

    // if (uiLayer.getElement("reset button")->getAsButton()->getActive())
    // {
    //     if (auto e = entityLayer.getEntity(0)->getComponent<MovementComponent>())
    //     {
    //         e->stats.speed = entityLayer.tManager.entityTemplates["player"].movement->speed;

    //         uiLayer.getElement("speed display")->getAsText()->setValue(std::to_string(toInt(e->stats.speed)));
    //     }
    // }

    // if (uiLayer.getElement("animation button")->getAsButton()->getActive())
    // {
    //     uiLayer.getElement("win 1")->setAnimation({0, 0}, {60, 300}, -1, 0, true);
    //     uiLayer.getElement("animation button")->setAnimation({0, 0}, {-75, 200}, -1, -1, true, false);
    //     uiLayer.getElement("animation button 2")->setAnimation({0, 0}, {25, 200}, -1, -1, true, false);
    // }
    
    // if (uiLayer.getElement("animation button 2")->getAsButton()->getActive())
    // {
    //     uiLayer.getElement("win 1")->setAnimation({0, 0}, {-280, 300}, -1, 0, true);
    //     uiLayer.getElement("animation button")->setAnimation({0, 0}, {25, 200}, -1, -1, true, false);
    //     uiLayer.getElement("animation button 2")->setAnimation({0, 0}, {-75, 200}, -1, -1, true, false);
    // }

    // if (uiLayer.getElement("action set attack button")->getAsButton()->getActive())
    // {
    //     if (entityLayer.player)
    //     {
    //         if (auto a = entityLayer.player->getComponent<ActionComponent>())
    //         {
    //             a->setAction("MAIN ACTION", {"attack!", 1.f, 0.f, 2.f});
    //         }
    //     }
    // }

    // if (uiLayer.getElement("action set heal button")->getAsButton()->getActive())
    // {
    //     if (entityLayer.player)
    //     {
    //         if (auto a = entityLayer.player->getComponent<ActionComponent>())
    //         {
    //             a->setAction("MAIN ACTION", {"heal!", -1.f, 1.f, 3.f});
    //         }
    //     }
    // }

    uiLayer.UIUpdate(dt);
}

void Scene::chunkLoadUpdate()
{
    chunkLayer.loadUpdate();
}

void Scene::draw()
{
    window->setView(camera.getView());

    drawBlame.clear();
    debugClock.restart();

    chunkLayer.draw(debugMode, debugChunkLayerView);
    drawBlame["DRAW_CHUNK_LAYER"] = debugClock.restart().asSeconds();
    
    entityLayer.draw(debugMode);
    drawBlame["DRAW_ENTITY_LAYER"] = debugClock.restart().asSeconds();

    uiLayer.draw(debugMode);
    drawBlame["DRAW_UI_LAYER"] = debugClock.restart().asSeconds();

    if (debugMode && debugLevel == 1) printBlameStats(drawBlame, "SCENE_DRAW");
}

void Scene::sceneInput(std::string control)
{
    if (control == "ZOOMIN")
    {
        camera.zoom(1);
    }
    else if (control == "ZOOMOUT")
    {
        camera.zoom(-1);
    }
    else if (control == "RESETZOOM")
    {
        camera.resetZoom();
    }
    else if (control == "TOGGLEFOCUS")
    {
        toggleFocus();
    }
    else if (control == "DEBUG_VIEW")
    {
        debugMode = !debugMode;
    }
    else if (control == "EXTRA 1")
    {
        entityLayer.addEntity(&entityLayer.tManager.entityTemplates["player"], true, game->getInput()->cursor->getGameCursorCoords());
        
        // debugChunkLayerView++;
    }
    else if (control == "EXTRA 2")
    {
        entityLayer.addEntity(&entityLayer.tManager.entityTemplates["box"], true, game->getInput()->cursor->getGameCursorCoords());

        // if (debugMode)
        // {
        //     debugLevel++;
    
        //     if (debugLevel > 1) debugLevel = 0;
        // }
    }
}

bool Scene::processActionRequest(Entity* actor, Action* action)
{
    if (actor)
    {
        if (auto a = actor->getComponent<ActionComponent>())
        {
            // CHECKS

            if (action->rangeMultiplier < 0.f || getDistance(actor->getComponent<PositionComponent>()->position.getPosition(), action->startPosition) <= a->range * action->rangeMultiplier)
            {
                // IS WITHIN RANGE

                if (dynamic_cast<MineAction*>(action))
                {
                    // TRYING TO MINE

                    if (auto t = game->getInput()->cursor->getSelectedTile())
                    {
                        // FOUND TARGETED TILE

                        for (auto& i : t->tags)
                        {
                            if (dynamic_cast<MineableTag*>(i.get()))
                            {
                                // TARGETED TILE CAN BE MINED, ACTION IS VALID
                                return true;
                            }
                        }
                    }

                    // EITHER THERE IS NO TILE SELECTED, OR THE TILE IS NOT MINEABLE
                    return false;
                }

                // ALL GENERAL CHECKS PASSED (NO SPECIFIC ACTION CHECKS APPLIED), REQUEST IS CONSIDERED VALID
                return true;
            }
        }
    }

    return false;
}

Camera* Scene::getCamera() { return &camera; }

void Scene::toggleFocus()
{
    if (!entityLayer.player) return;

    if (camera.getFocus() == nullptr)
    {
        if (!entityLayer.player->getComponent<ControlComponent>()) entityLayer.player->addComponent<ControlComponent>(entityLayer.player);
        camera.setFocus(entityLayer.player);
    }
    else
    {
        entityLayer.player->removeComponent<ControlComponent>();
        camera.removeFocus();
    }
}

EntityLayer* Scene::getEntityLayer() { return &entityLayer; }

UILayer* Scene::getUILayer() { return &uiLayer; }

ChunkLayer* Scene::getChunkLayer() { return &chunkLayer; }

sf::Vector2i Scene::getWorldChunkOrigin() { return worldChunkOrigin; }

void Scene::adjustWorldChunkOrigin(sf::Vector2i amount)
{
    worldChunkOrigin += amount;

    float chunkLength = game->getSettings()->tile_size * game->getSettings()->chunk_size;

    std::vector<Entity*> entitiesWithPosition = entityLayer.getEntitiesWithComponent<PositionComponent>();
    for (auto e : entitiesWithPosition)
    {
        auto p = e->getComponent<PositionComponent>();

        p->position.changePosition({amount.x * -chunkLength, amount.y * -chunkLength});
    }

    auto allChunks = chunkLayer.getAllLoadedChunks();
    for (auto& pair : *allChunks)
    {
        Chunk* c = pair.second.get();

        sf::Vector2i chunkPos = c->getChunkPosition();
        c->worldPosition = {(chunkPos.x - worldChunkOrigin.x) * chunkLength, (chunkPos.y - worldChunkOrigin.y) * chunkLength};

        c->createAllTileVerts();

        for (auto& bg : c->bgObjects)
        {
            bg.bottom += amount.y * -chunkLength;
            bg.rect.position += {amount.x * -chunkLength, amount.y * -chunkLength};
        }
    }
}