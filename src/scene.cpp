#include "scene.hpp"
#include "game.hpp"

#include <memory>

Scene::Scene() {}

void Scene::init(Game* game)
{
    this->game = game;

    window = game->getWindow();

    assetManager = game->getAssetManager();
    

    
    entityLayer.init(game);

    uiLayer.init(game, &camera);

    tileLayer.init(game);

    camera.init(game, true, {0, 0}, toV2F(window->getSize()), entityLayer.getEntity(1));

    rect.setSize({100.f, 100.f});
    rect.setFillColor(sf::Color::Red);
    rect.setOrigin({50.f, 50.f});
    rect.setPosition(toV2F(window->getSize().x / 2, window->getSize().y / 2));

    outline.setSize({480, 480});
    outline.setOutlineColor(sf::Color::Red);
    outline.setOutlineThickness(5.f);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOrigin({240.f, 240.f});
    outline.setPosition(toV2F(window->getSize().x / 2, window->getSize().y / 2));
}

void Scene::tick()
{   
    entityLayer.tick();

    tileLayer.tick();
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

    if (uiLayer.getElement("animation button")->getAsButton()->getActive())
    {
        uiLayer.getElement("win 1")->setAnimation({0, 0}, {80, 120}, -1, 0, true);
        // uiLayer.getElement("many win parent")->setAnimation({0, 0}, {-50, 50}, -1, 1, true);
    }
    
    if (uiLayer.getElement("animation button 2")->getAsButton()->getActive())
    {
        uiLayer.getElement("win 1")->setAnimation({0, 0}, {-300, 120}, -1, 0, true);
        // uiLayer.getElement("many win parent")->setAnimation({0, 0}, {50, 50}, -1, 0, true);
    }

    uiLayer.UIUpdate(dt);
}

void Scene::draw()
{
    tileLayer.draw();

    window->draw(rect);
    window->draw(outline);
    
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
        camera.setFocus(entityLayer.getEntity(1));
        entityLayer.getEntity(1)->getMotion()->controlling = true;
    }
    else
    {
        camera.removeFocus();
        entityLayer.getEntity(1)->getMotion()->controlling = false;
    }
}

UILayer* Scene::getUILayer() { return &uiLayer; }

TileLayer* Scene::getTileLayer() { return &tileLayer; }