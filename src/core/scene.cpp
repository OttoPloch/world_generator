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
    camera.tick();

    chunkLayer.tick();
    
    entityLayer.tick();

    uiLayer.tick();

    // TEMP
    sf::Vector2i mouseChunkPos = worldToChunkPosition(game, window->getWindow().mapPixelToCoords(sf::Mouse::getPosition(window->getWindow())));
    // uiLayer.getElement("mouse chunk pos display")->getAsText()->setValue(std::to_string(mouseChunkPos.x) + ", " + std::to_string(mouseChunkPos.y));
    auto element = uiLayer.getElement("debug text display");
    if (element)
    {
        if (auto chunkPosText = element->getComponent<TextComponent>("mouse chunk pos text"))
        {
            chunkPosText->setText("Mouse Chunk Pos: " + std::to_string(mouseChunkPos.x) + ", " + std::to_string(mouseChunkPos.y));
        }
    
        sf::Vector2f mouseWorldPos = game->getInput()->getMouseCoords();
        sf::Vector2f mouseLocalWorldPos = {std::fmod(mouseWorldPos.x, toFloat(game->getSettings()->chunk_size) * game->getSettings()->tile_size), std::fmod(mouseWorldPos.y, toFloat(game->getSettings()->chunk_size) * game->getSettings()->tile_size)};
        sf::Vector2i mouseLocalPos = {toInt(std::floor(mouseLocalWorldPos.x / game->getSettings()->tile_size)), toInt(std::floor(mouseLocalWorldPos.y / game->getSettings()->tile_size))};
        Chunk* mouseChunk = chunkLayer.getChunk(mouseChunkPos);
        
        std::string mouseTileType = "nwwwwwwwwwwwwwwwwwwwwone";

        if (mouseChunk && mouseChunk->state == ChunkState::ACTIVE)
        {
            Tile* mouseTile = mouseChunk->getTile(mouseLocalPos.x, mouseLocalPos.y);
    
            if (mouseTile)
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
            
                mouseTileType = typesToStrings[mouseTile->type];
            }
        }

        if (auto tileTypeText = element->getComponent<TextComponent>("mouse tile type text"))
        {
            tileTypeText->setText("Mouse Tile Type: " + mouseTileType);
        }
    }
    /////
}

void Scene::update(float dt)
{
    debugClock.restart();

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

    debugClock.restart();

    chunkLayer.draw(debugMode, debugChunkLayerView);
    drawBlame["DRAW_CHUNK_LAYER"] = debugClock.restart().asSeconds();
    
    entityLayer.draw(debugMode);
    drawBlame["DRAW_ENTITY_LAYER"] = debugClock.restart().asSeconds();

    uiLayer.draw(debugMode);
    drawBlame["DRAW_UI_LAYER"] = debugClock.restart().asSeconds();

    if (debugMode && debugLevel == 1) printBlameStats(drawBlame, "SCENE_DRAW");
}

void Scene::sceneInput(std::string control, bool justPressed)
{
    if (control == "RESETZOOM" && justPressed)
    {
        camera.resetZoom();
    }
    else if (control == "TOGGLEFOCUS" && justPressed)
    {
        toggleFocus();
    }
    else if (control == "DEBUG_VIEW" && justPressed)
    {
        debugMode = !debugMode;
    }
    else if (control == "ZOOMIN" && justPressed)
    {
        camera.zoom(1);
    }
    else if (control == "ZOOMOUT" && justPressed)
    {
        camera.zoom(-1);
    }
    else if (control == "INTERACT" && justPressed)
    {
        //uiLayer.interactiveUIManager.click();
    }
    else if (control == "EXTRA 1" && justPressed)
    {
        debugChunkLayerView++;
    }
    else if (control == "EXTRA 2" && justPressed)
    {
        if (debugMode)
        {
            debugLevel++;
    
            if (debugLevel > 1) debugLevel = 0;
        }
    }
    else if (control == "MAIN ACTION")
    {
        if (!uiLayer.checkUICollision())
        {
            if (entityLayer.player)
            {
                if (auto a = entityLayer.player->getComponent<ActionComponent>())
                {
                    a->startAction(control);
                }
            }
        }
    }
    else if (control == "SECONDARY ACTION")
    {
        if (!uiLayer.checkUICollision())
        {
            if (entityLayer.player)
            {
                if (auto a = entityLayer.player->getComponent<ActionComponent>())
                {
                    a->startAction(control);
                }
            }
        }
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

                    sf::Vector2f mouseWorldPos = game->getInput()->getMouseCoords();

                    if (auto chunk = chunkLayer.getChunk(worldToChunkPosition(game, mouseWorldPos)))
                    {
                        // IN A VALID CHUNK

                        sf::Vector2i tilePos = worldToTilePosition(game, mouseWorldPos);
                        tilePos = {tilePos.x % game->getSettings()->chunk_size, tilePos.y % game->getSettings()->chunk_size};

                        if (auto t = chunk->getTile(tilePos.x, tilePos.y))
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
                    }

                    // EITHER THE CHUNK OR TILE DOESN'T EXIST, OR THERE THE TILE IS NOT MINEABLE
                    return false;
                }

                // ALL CHECKS PASSED, REQUEST IS VALID
                return true;
            }
        }
    }

    return false;
}

Camera* Scene::getCamera() { return &camera; }

void Scene::toggleFocus()
{
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