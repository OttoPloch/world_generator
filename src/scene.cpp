#include "scene.hpp"

Scene::Scene() {}

void Scene::init(Window* window, AssetManager* assetManager)
{
    camera.init(window, true, {0, 0}, toV2F(window->getSize()));

    thing.create({475, 475});
    thing.giveSprite(assetManager->getTexture("pixel"), {50, 50});

    thing2.create({0, 0});
    thing2.giveSprite(assetManager->getTexture("shaq"), {300, 300});
    thing2.giveMotion();

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
    thing.tick();
    thing2.tick();
}

void Scene::update(float dt)
{
    camera.update(dt);
}

void Scene::draw(sf::RenderWindow* window)
{
    window->draw(rect);
    window->draw(outline);

    thing.draw(*window);
    thing2.draw(*window);
}

Camera* Scene::getCamera()
{
    return &camera;
}