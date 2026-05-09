#include "scene.hpp"
#include "game.hpp"
#include "input.hpp"
#include "../entities/states.hpp"
#include "../utils/utils.hpp"
#include "../entities/components/movement_component.hpp"
#include "../entities/components/control_component.hpp"
#include "../entities/components/collision_component.hpp"

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

    uiLayer.init(game, &camera);

    chunkLayer.init(game);

    camera.init(game, false, {0, 0}, toV2F(window->getSize()), entityLayer.getEntity(0));

    debugView = false;
    debugChunkLayerView = -1;
}

void Scene::tick()
{   
    camera.tick();

    entityLayer.tick();

    chunkLayer.tick();

    // TEMP
    sf::Vector2i mouseChunkPos = worldToChunkPosition(game, window->getWindow().mapPixelToCoords(sf::Mouse::getPosition(window->getWindow())));
    uiLayer.getElement("mouse chunk pos display")->getAsText()->setValue(std::to_string(mouseChunkPos.x) + ", " + std::to_string(mouseChunkPos.y));
    
    sf::Vector2f mouseWorldPos = window->getWindow().mapPixelToCoords(sf::Mouse::getPosition(window->getWindow()));
    sf::Vector2f mouseLocalWorldPos = {std::fmod(mouseWorldPos.x, toFloat(game->getSettings()->chunk_size) * game->getSettings()->tile_size), std::fmod(mouseWorldPos.y, toFloat(game->getSettings()->chunk_size) * game->getSettings()->tile_size)};
    sf::Vector2i mouseLocalPos = {toInt(std::floor(mouseLocalWorldPos.x / game->getSettings()->tile_size)), toInt(std::floor(mouseLocalWorldPos.y / game->getSettings()->tile_size))};
    Chunk* mouseChunk = chunkLayer.getChunk(mouseChunkPos);
    
    if (mouseChunk && mouseChunk->state == ChunkState::ACTIVE)
    {
        int tileZ = game->getSettings()->maxTileZ;
        Tile* mouseTile = mouseChunk->getTile(mouseLocalPos.x, mouseLocalPos.y, tileZ);
     
        while (mouseTile->type == TileType::AIR && tileZ >= 0)
        {
            tileZ--;
            mouseTile = mouseChunk->getTile(mouseLocalPos.x, mouseLocalPos.y, tileZ);
        }

        std::map<TileType, std::string> typesToStrings {
            {TileType::AIR, "air"},
            {TileType::WATER, "water"},
            {TileType::GRASS, "grass"},
            {TileType::STONE, "stone"},
            {TileType::LAVA, "lava"},
            {TileType::COBBLE, "cobble"},
            {TileType::PINK, "pink"},
        };
    
        uiLayer.getElement("mouse tile type display")->getAsText()->setValue(typesToStrings[mouseTile->type]);
    }
    else
    {
        uiLayer.getElement("mouse tile type display")->getAsText()->setValue("none");
    }
    ///////
}

void Scene::update(float dt)
{       
    chunkLayer.update(dt);
    
    entityLayer.update(dt);
}

void Scene::UIUpdate(float dt)
{
    if (uiLayer.getElement("faster button")->getAsButton()->getActive())
    {
        if (auto e = entityLayer.getEntity(0)->getComponent<MovementComponent>())
        {
            e->stats.speed += 2;
            
            uiLayer.getElement("speed display")->getAsText()->setValue(std::to_string(toInt(e->stats.speed)));
        }        
    }
    
    if (uiLayer.getElement("slower button")->getAsButton()->getActive())
    {
        if (auto e = entityLayer.getEntity(0)->getComponent<MovementComponent>())
        {
            e->stats.speed -= 2;

            uiLayer.getElement("speed display")->getAsText()->setValue(std::to_string(toInt(e->stats.speed)));
        }
    }

    if (uiLayer.getElement("reset button")->getAsButton()->getActive())
    {
        if (auto e = entityLayer.getEntity(0)->getComponent<MovementComponent>())
        {
            // TEMP, TODO: replace constant with value from entity template for player.
            e->stats.speed = 2;

            uiLayer.getElement("speed display")->getAsText()->setValue(std::to_string(toInt(e->stats.speed)));
        }
    }

    if (uiLayer.getElement("animation button")->getAsButton()->getActive())
    {
        uiLayer.getElement("win 1")->setAnimation({0, 0}, {60, 300}, -1, 0, true);
        uiLayer.getElement("animation button")->setAnimation({0, 0}, {-75, 200}, -1, -1, true, false);
        uiLayer.getElement("animation button 2")->setAnimation({0, 0}, {25, 200}, -1, -1, true, false);
    }
    
    if (uiLayer.getElement("animation button 2")->getAsButton()->getActive())
    {
        uiLayer.getElement("win 1")->setAnimation({0, 0}, {-280, 300}, -1, 0, true);
        uiLayer.getElement("animation button")->setAnimation({0, 0}, {25, 200}, -1, -1, true, false);
        uiLayer.getElement("animation button 2")->setAnimation({0, 0}, {-75, 200}, -1, -1, true, false);
    }

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
        uiLayer.interactiveUIManager.click();
    }
    else if (control == "EXTRA 1" && justPressed)
    {
        debugChunkLayerView++;
    }
    else if (control == "MAIN ACTION")
    {
    }
}

Camera* Scene::getCamera() { return &camera; }

void Scene::toggleFocus()
{
    if (camera.getFocus() == nullptr)
    {
        Entity* e = entityLayer.getEntity(0);
        if (!e->getComponent<ControlComponent>()) e->addComponent<ControlComponent>(e);
        camera.setFocus(e);
    }
    else
    {
        Entity* e = entityLayer.getEntity(0);
        e->removeComponent<ControlComponent>();
        camera.removeFocus();
    }
}

EntityLayer* Scene::getEntityLayer() { return &entityLayer; }

UILayer* Scene::getUILayer() { return &uiLayer; }

ChunkLayer* Scene::getChunkLayer() { return &chunkLayer; }