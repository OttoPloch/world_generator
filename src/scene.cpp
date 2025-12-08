#include "scene.hpp"

Scene::Scene() {}

void Scene::init(Window* window, AssetManager* assetManager)
{
    this->window = window;

    this->assetManager = assetManager;
    
    IDCounter = 0;

    thing.create(IDCounter, {475, 475});
    thing.giveSprite(assetManager->getTexture("pixel"), {50, 50});
    thing.giveCollision(&entities, true);
    
    IDCounter++;

    thing2.create(IDCounter, {0, 0});
    thing2.giveSprite(assetManager->getTexture("shaq"), {300, 300});
    thing2.giveMotion(true);
    thing2.giveCollision(&entities, false);
    
    IDCounter++;

    entities.clear();
    entities.push_back(std::move(thing));
    entities.push_back(std::move(thing2));
    
    camera.init(window, true, {0, 0}, toV2F(window->getSize()), &entities[1]);

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

    // realPos.setFillColor(sf::Color::Blue);
    // realPos.setRadius(10.f);
    // realPos.setOrigin({10.f, 10.f});
    // realPos.setPosition(entities[1].getPosition());
}

void Scene::tick()
{
    entities[0].tick();
    entities[1].tick();

    // realPos.setPosition(entities[1].getPosition());
}

void Scene::update(float dt)
{
    camera.update(dt);

    window->setView(camera.getView());

    entities[0].update(dt);
    entities[1].update(dt);
}

void Scene::draw()
{
    window->draw(rect);
    window->draw(outline);

    entities[0].draw(window->getWindow());
    entities[1].draw(window->getWindow());

    // window->draw(realPos);
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
        camera.setFocus(&entities[1]);
        entities[1].getMotion()->controlling = true;
    }
    else
    {
        camera.removeFocus();
        entities[1].getMotion()->controlling = false;
    }
}