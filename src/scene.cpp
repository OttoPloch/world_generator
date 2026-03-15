#include "scene.hpp"
#include "game.hpp"
#include "input.hpp"
#include "utils.hpp"

#include <SFML/Graphics/CircleShape.hpp>
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

    camera.init(game, true, {0, 0}, toV2F(window->getSize()));

    debugView = false;
}

void Scene::tick()
{   
    camera.tick();

    entityLayer.tick();

    chunkLayer.tick();

    sf::Vector2i mouseChunkPos = worldToChunkPosition(game, window->getWindow().mapPixelToCoords(sf::Mouse::getPosition(window->getWindow())));
    uiLayer.getElement("mouse chunk pos display")->getAsText()->setValue(std::to_string(mouseChunkPos.x) + ", " + std::to_string(mouseChunkPos.y));
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
        game->getGamerules()->setRule("moveSpeed", gamerule(game->getGamerules()->getRule("moveSpeed", "player").valueFloat + 3.f, 0, false, ""), "player");
    
        uiLayer.getElement("speed display")->getAsText()->setValue(std::to_string(toInt(game->getGamerules()->getRule("moveSpeed", "player").valueFloat)));
    }
    
    if (uiLayer.getElement("slower button")->getAsButton()->getActive())
    {
        game->getGamerules()->setRule("moveSpeed", gamerule(std::max(game->getGamerules()->getRule("moveSpeed", "player").valueFloat - 3.f, .1f), 0, false, ""), "player");
        
        uiLayer.getElement("speed display")->getAsText()->setValue(std::to_string(toInt(game->getGamerules()->getRule("moveSpeed", "player").valueFloat)));
    }

    if (uiLayer.getElement("reset button")->getAsButton()->getActive())
    {
        game->getGamerules()->setRule("moveSpeed", gamerule(5.f, 0, false, ""), "player");

        uiLayer.getElement("speed display")->getAsText()->setValue(std::to_string(toInt(game->getGamerules()->getRule("moveSpeed", "player").valueFloat)));
    }

    if (uiLayer.getElement("animation button")->getAsButton()->getActive())
    {
        uiLayer.getElement("win 1")->setAnimation({0, 0}, {60, 250}, -1, 0, true);
        uiLayer.getElement("animation button")->setAnimation({0, 0}, {-75, 140}, -1, -1, true, false);
        uiLayer.getElement("animation button 2")->setAnimation({0, 0}, {25, 140}, -1, -1, true, false);
    }
    
    if (uiLayer.getElement("animation button 2")->getAsButton()->getActive())
    {
        uiLayer.getElement("win 1")->setAnimation({0, 0}, {-280, 250}, -1, 0, true);
        uiLayer.getElement("animation button")->setAnimation({0, 0}, {25, 140}, -1, -1, true, false);
        uiLayer.getElement("animation button 2")->setAnimation({0, 0}, {-75, 140}, -1, -1, true, false);
    }

    uiLayer.UIUpdate(dt);
}

void Scene::chunkLoadUpdate()
{
    chunkLayer.loadUpdate();
}

void Scene::draw(float alpha)
{
    window->setView(camera.getView());

    chunkLayer.draw(debugView);
    entityLayer.draw(alpha);
    uiLayer.draw();
}

void Scene::sceneInput(std::string control)
{
    if (control == "RESETZOOM")
    {
        camera.resetZoom();
    }
    else if (control == "TOGGLEFOCUS")
    {
        toggleFocus();
    }
    else if (control == "DEBUG_VIEW")
    {
        debugView = !debugView;
    }
    else if (control == "ZOOMIN")
    {
        camera.zoom(1);
    }
    else if (control == "ZOOMOUT")
    {
        camera.zoom(-1);
    }
    else if (control == "INTERACT")
    {
        uiLayer.interactiveUIManager.click();
    }
}

Camera* Scene::getCamera() { return &camera; }

void Scene::toggleFocus()
{
    if (camera.getFocus() == nullptr)
    {
        camera.setFocus(entityLayer.getEntity(0));
    }
    else
    {
        camera.removeFocus();
    }
}

EntityLayer* Scene::getEntityLayer() { return &entityLayer; }

UILayer* Scene::getUILayer() { return &uiLayer; }

ChunkLayer* Scene::getChunkLayer() { return &chunkLayer; }