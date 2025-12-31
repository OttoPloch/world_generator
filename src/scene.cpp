#include "scene.hpp"
#include "rect_types.hpp"
#include "game.hpp"

#include <memory>

Scene::Scene() {}

void Scene::init(Game* game)
{
    window = game->getWindow();

    assetManager = game->getAssetManager();
    
    IDCounter = 0;

    
    
    entityLayer.init(game);

    entityLayer.addEntity(getNewID(), {475, 475});
    entityLayer.addEntity(getNewID(), {0, 0});
    entityLayer.addEntity(getNewID(), {900, 900});
    entityLayer.addEntity(getNewID(), {100, 500});
    entityLayer.addEntity(getNewID(), {0, 0});
    entityLayer.addEntity(getNewID(), {-400, 100});
    entityLayer.addEntity(getNewID(), {-400, 250});
    entityLayer.addEntity(getNewID(), {-400, 400});
    entityLayer.addEntity(getNewID(), {-400, 550});
    entityLayer.addEntity(getNewID(), {-400, 700});
    entityLayer.addEntity(getNewID(), {-200, 700});

    entityLayer.giveEntitySprite(0, assetManager->getTexture("pixel"), {50, 50}, -1);

    entityLayer.giveEntitySprite(1, assetManager->getTexture("IDLE_smaller"), {24 * 10, 21 * 10});
    entityLayer.getEntity(1)->getSprite()->giveAnimationSet(assetManager->getAnimSet("player"));
    entityLayer.giveEntityMotion(1, 1.f, true);
    entityLayer.giveEntityCollision(1, "player", ACTIVE, {}, {0, 0.2}, {0.3, 0.3});

    entityLayer.giveEntitySprite(2, assetManager->getTexture("dr bee"), {200, 200});
    entityLayer.giveEntityCollision(2, "enemy", STATIC);

    entityLayer.giveEntitySprite(3, assetManager->getTexture("bush"), {200, 120});
    entityLayer.giveEntityCollision(3, "obstacle", STATIC, {}, {0, 0.2f}, {.7f, .6f});

    entityLayer.giveEntitySprite(4, assetManager->getTexture("IDLE_smaller"), {24 * 11, 21 * 11});
    entityLayer.getEntity(4)->getSprite()->giveAnimation(assetManager->getAnimation("knight_idle"));

    entityLayer.giveEntitySprite(5, assetManager->getTexture("pixel"), {100, 100}, -2);
    entityLayer.getEntity(5)->getSprite()->giveAnimation(assetManager->getAnimation("dot_left"));

    entityLayer.giveEntitySprite(6, assetManager->getTexture("pixel"), {100, 100}, -2);
    entityLayer.getEntity(6)->getSprite()->giveAnimation(assetManager->getAnimation("dot_right"));

    entityLayer.giveEntitySprite(7, assetManager->getTexture("pixel"), {100, 100}, -2);
    entityLayer.getEntity(7)->getSprite()->giveAnimation(assetManager->getAnimation("dot_up"));

    entityLayer.giveEntitySprite(8, assetManager->getTexture("pixel"), {100, 100}, -2);
    entityLayer.getEntity(8)->getSprite()->giveAnimation(assetManager->getAnimation("dot_down"));

    entityLayer.giveEntitySprite(9, assetManager->getTexture("pixel"), {100, 100}, -2);
    entityLayer.getEntity(9)->getSprite()->giveAnimation(assetManager->getAnimation("dot_idle"));

    entityLayer.giveEntitySprite(10, assetManager->getTexture("crate"), {150, 150});
    entityLayer.giveEntityMotion(10, 1.f);
    entityLayer.giveEntityCollision(10, "crate", MOVABLE);

    uiLayer.init(game, &camera);

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
}

void Scene::update(float dt)
{
    camera.update(dt);

    window->setView(camera.getView());

    entityLayer.update(dt);
}

void Scene::draw()
{
    window->draw(rect);
    window->draw(outline);

    entityLayer.draw();

    uiLayer.draw();
}

void Scene::sceneInput(sf::Keyboard::Key key)
{
    switch (key)
    {
        case sf::Keyboard::Key::Enter:
            camera.resetZoom();
            break;
        case sf::Keyboard::Key::F1:
            toggleFocus();
            break;
        default:
            break;
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

int Scene::getNewID()
{
    IDCounter++;

    return IDCounter - 1;
}

UILayer* Scene::getUILayer() { return &uiLayer; }