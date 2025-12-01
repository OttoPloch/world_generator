#include "scene.hpp"

Scene::Scene() {}

void Scene::init(Window* window, AssetManager* assetManager)
{
    this->window = window;

    this->assetManager = assetManager;

    camera.init(window, true, {0, 0}, toV2F(window->getSize()), &thing2);

    thing.create({475, 475});
    thing.giveSprite(assetManager->getTexture("pixel"), {50, 50});

    thing2.create({0, 0});
    thing2.giveSprite(assetManager->getTexture("shaq"), {300, 300});
    thing2.giveMotion(true);

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

    window->setView(camera.getView());

    thing.update(dt);
    thing2.update(dt);
}

void Scene::draw()
{
    window->draw(rect);
    window->draw(outline);

    thing.draw(window->getWindow());
    thing2.draw(window->getWindow());
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
        camera.setFocus(&thing2);
        thing2.getMotion()->controlling = true;
    }
    else
    {
        camera.removeFocus();
        thing2.getMotion()->controlling = false;
    }
}