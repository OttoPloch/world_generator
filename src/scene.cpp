#include "scene.hpp"

Scene::Scene() {}

void Scene::init(Window* window, AssetManager* assetManager)
{
    this->window = window;

    this->assetManager = assetManager;
    
    IDCounter = 0;

    entities.clear();
    entities.push_back(Entity(getNewID(), {475, 475}));
    entities.push_back(Entity(getNewID(), {0, 0}));
    entities.push_back(Entity(getNewID(), {900, 900}));

    entities[0].giveSprite(assetManager->getTexture("pixel"), {50, 50});
    entities[0].giveCollision(&entities, false);

    entities[1].giveSprite(assetManager->getTexture("shaq"), {300, 300});
    entities[1].giveMotion(true);
    entities[1].giveCollision(&entities, true);

    entities[2].giveSprite(assetManager->getTexture("dr bee"), {200, 200});
    entities[2].giveCollision(&entities, false);
    
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
    for (int i = 0; i < entities.size(); i++)
    {
        entities[i].tick();
    }

    // realPos.setPosition(entities[1].getPosition());
}

void Scene::update(float dt)
{
    camera.update(dt);

    window->setView(camera.getView());

    for (int i = 0; i < entities.size(); i++)
    {
        entities[i].update(dt);
    }
}

void Scene::draw()
{
    window->draw(rect);
    window->draw(outline);

    for (int i = 0; i < entities.size(); i++)
    {
        entities[i].draw(window->getWindow());
    }

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

int Scene::getNewID()
{
    IDCounter++;

    return IDCounter - 1;
}