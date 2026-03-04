#include "scene.hpp"
#include "game.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
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
}

void Scene::tick()
{   
    entityLayer.tick();

    chunkLayer.tick();
}

void Scene::update(float dt)
{
    camera.update(dt);
    
    window->setView(camera.getView());
    
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
        uiLayer.getElement("animation button")->setAnimation({0, 0}, {-75, 130}, -1, -1, true, false);
        uiLayer.getElement("animation button 2")->setAnimation({0, 0}, {25, 130}, -1, -1, true, false);
    }
    
    if (uiLayer.getElement("animation button 2")->getAsButton()->getActive())
    {
        uiLayer.getElement("win 1")->setAnimation({0, 0}, {-280, 250}, -1, 0, true);
        uiLayer.getElement("animation button")->setAnimation({0, 0}, {25, 130}, -1, -1, true, false);
        uiLayer.getElement("animation button 2")->setAnimation({0, 0}, {-75, 130}, -1, -1, true, false);
    }

    uiLayer.UIUpdate(dt);
}

void Scene::chunkUpdate()
{
    chunkLayer.update();
}

void Scene::draw()
{
    chunkLayer.draw(true);
    entityLayer.draw();
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
        //camera.setFocus(entityLayer.getEntity(1));
        //entityLayer.getEntity(1)->getMotion()->controlling = true;
    }
    else
    {
        camera.removeFocus();
        //entityLayer.getEntity(1)->getMotion()->controlling = false;
    }
}

EntityLayer* Scene::getEntityLayer() { return &entityLayer; }

UILayer* Scene::getUILayer() { return &uiLayer; }

ChunkLayer* Scene::getChunkLayer() { return &chunkLayer; }