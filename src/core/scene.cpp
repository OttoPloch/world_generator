#include "scene.hpp"
#include "game.hpp"
#include "input.hpp"
#include "../entities/states.hpp"
#include "../utils/utils.hpp"
#include "../entities/components/movement_component.hpp"
#include "../entities/components/control_component.hpp"
#include "../entities/components/collision_component.hpp"
#include "../entities/components/action_component.hpp"
#include "../entities/actions/action.hpp"
#include "../entities/actions/mine_action.hpp"

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

    camera.init(game, false, {0, 0}, toV2F(window->getSize()), entityLayer.getEntity(0));

    uiLayer.init(game, &camera);

    chunkLayer.init(game);

    debugView = false;
    debugChunkLayerView = -1;
}

void Scene::tick()
{   
    camera.tick();

    entityLayer.tick();

    chunkLayer.tick();

    // TEMP
    // sf::Vector2i mouseChunkPos = worldToChunkPosition(game, window->getWindow().mapPixelToCoords(sf::Mouse::getPosition(window->getWindow())));
    // uiLayer.getElement("mouse chunk pos display")->getAsText()->setValue(std::to_string(mouseChunkPos.x) + ", " + std::to_string(mouseChunkPos.y));
    
    // sf::Vector2f mouseWorldPos = game->getInput()->getMouseWorldPos();
    // sf::Vector2f mouseLocalWorldPos = {std::fmod(mouseWorldPos.x, toFloat(game->getSettings()->chunk_size) * game->getSettings()->tile_size), std::fmod(mouseWorldPos.y, toFloat(game->getSettings()->chunk_size) * game->getSettings()->tile_size)};
    // sf::Vector2i mouseLocalPos = {toInt(std::floor(mouseLocalWorldPos.x / game->getSettings()->tile_size)), toInt(std::floor(mouseLocalWorldPos.y / game->getSettings()->tile_size))};
    // Chunk* mouseChunk = chunkLayer.getChunk(mouseChunkPos);
    
    // if (mouseChunk && mouseChunk->state == ChunkState::ACTIVE)
    // {
    //     Tile* mouseTile = mouseChunk->getTile(mouseLocalPos.x, mouseLocalPos.y);

    //     if (mouseTile)
    //     {
    //         std::map<TileType, std::string> typesToStrings {
    //             {TileType::AIR, "air"},
    //             {TileType::WATER, "water"},
    //             {TileType::GRASS, "grass"},
    //             {TileType::STONE, "stone"},
    //             {TileType::LAVA, "lava"},
    //             {TileType::COBBLE, "cobble"},
    //             {TileType::PINK, "pink"},
    //         };
        
    //         uiLayer.getElement("mouse tile type display")->getAsText()->setValue(typesToStrings[mouseTile->type]);
    //     }
    //     else
    //     {
    //         uiLayer.getElement("mouse tile type display")->getAsText()->setValue("none");
    //     }
    // }
    // else
    // {
    //     uiLayer.getElement("mouse tile type display")->getAsText()->setValue("none");
    // }
    ///////
}

void Scene::update(float dt)
{       
    chunkLayer.update(dt);

    entityLayer.update(dt);
}

void Scene::UIUpdate(float dt)
{
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

    chunkLayer.draw(debugView, debugChunkLayerView);
    entityLayer.draw(debugView);
    uiLayer.draw();
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
        debugView = !debugView;
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

            if (action->rangeMultiplier < 0.f || getDistance(actor->getPosition(), action->startPosition) <= a->range * action->rangeMultiplier)
            {
                // IS WITHIN RANGE

                if (dynamic_cast<MineAction*>(action))
                {
                    // TRYING TO MINE

                    sf::Vector2f mouseWorldPos = game->getInput()->getMouseWorldPos();

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
                                if (i->name == "MINE")
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
        Entity* e = entityLayer.getEntity(0);
        entityLayer.player = e;
        if (!e->getComponent<ControlComponent>()) e->addComponent<ControlComponent>(e);
        camera.setFocus(e);
    }
    else
    {
        Entity* e = entityLayer.getEntity(0);
        e->removeComponent<ControlComponent>();
        camera.removeFocus();
        entityLayer.player = nullptr;
    }
}

EntityLayer* Scene::getEntityLayer() { return &entityLayer; }

UILayer* Scene::getUILayer() { return &uiLayer; }

ChunkLayer* Scene::getChunkLayer() { return &chunkLayer; }