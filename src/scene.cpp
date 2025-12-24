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
    entities.push_back(Entity(getNewID(), {100, 500}));
    entities.push_back(Entity(getNewID(), {0, 0}));
    entities.push_back(Entity(getNewID(), {-400, 100}));
    entities.push_back(Entity(getNewID(), {-400, 250}));
    entities.push_back(Entity(getNewID(), {-400, 400}));
    entities.push_back(Entity(getNewID(), {-400, 550}));
    entities.push_back(Entity(getNewID(), {-400, 700}));

    entities[0].giveSprite(assetManager->getTexture("pixel"), {50, 50}, -1);
    // entities[0].giveCollision(&entities, false, "pixel");

    entities[1].giveSprite(assetManager->getTexture("IDLE_smaller"), {24 * 10, 21 * 10});
    entities[1].getSprite()->giveAnimationSet(assetManager->getAnimSet("player"));
    entities[1].giveMotion(true);
    entities[1].giveCollision(&entities, true, "player", {0, 0.2}, {0.3, 0.3});

    entities[2].giveSprite(assetManager->getTexture("dr bee"), {200, 200});
    entities[2].giveCollision(&entities, false, "enemy");

    entities[3].giveSprite(assetManager->getTexture("bush"), {200, 120});
    entities[3].giveCollision(&entities, false, "obstacle", {0, 0.2f}, {.7f, .6f});

    entities[4].giveSprite(assetManager->getTexture("IDLE_smaller"), {24 * 11, 21 * 11});
    entities[4].getSprite()->giveAnimation(assetManager->getAnimation("knight_idle"));

    entities[5].giveSprite(assetManager->getTexture("pixel"), {100, 100}, -1);
    entities[5].getSprite()->giveAnimation(assetManager->getAnimation("dot_left"));

    entities[6].giveSprite(assetManager->getTexture("pixel"), {100, 100}, -1);
    entities[6].getSprite()->giveAnimation(assetManager->getAnimation("dot_right"));

    entities[7].giveSprite(assetManager->getTexture("pixel"), {100, 100}, -1);
    entities[7].getSprite()->giveAnimation(assetManager->getAnimation("dot_up"));

    entities[8].giveSprite(assetManager->getTexture("pixel"), {100, 100}, -1);
    entities[8].getSprite()->giveAnimation(assetManager->getAnimation("dot_down"));

    entities[9].giveSprite(assetManager->getTexture("pixel"), {100, 100}, -1);
    entities[9].getSprite()->giveAnimation(assetManager->getAnimation("dot_idle"));

    // TODO: optional, if i want to change whether an entity has a sprite or its z value,
    // then i would need to modify this whenever that happens.
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i].getSprite())
        {
            entitiesZMap[entities[i].getSprite()->getZ()].push_back(&entities[i]);
        }
    }

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
}

void Scene::tick()
{
    for (int i = 0; i < entities.size(); i++)
    {
        entities[i].tick();
    }
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

    for (auto i : entitiesZMap)
    {
        std::vector<Entity*>* vec = &i.second;

        sortEntitiesByY(vec, 0, vec->size() - 1);

        for (int j = 0; j < vec->size(); j++)
        {
            (*vec)[j]->draw(window->getWindow());

            // Entity* entity = (*vec)[j];
            // Sprite* sprite = entity->getSprite();
            // sf::Sprite sprite2 = sprite->getSprite();
            // sf::Vector2f size = toV2F(sprite2.getTextureRect().size);
            // sf::Vector2f size2 = sprite->getSize();

            // sf::RectangleShape spriteOutline(size);
            // sf::RectangleShape spriteOutline2(size2);

            // spriteOutline.setOrigin({size.x / 2.f, size.y / 2.f});
            // spriteOutline.setPosition((*vec)[j]->getPosition());
            
            // spriteOutline2.setOrigin({size2.x / 2.f, size2.y / 2.f});
            // spriteOutline2.setPosition((*vec)[j]->getPosition());
    
            // spriteOutline.setFillColor(sf::Color::Transparent);
            // spriteOutline.setOutlineColor(sf::Color::Red);
            // spriteOutline.setOutlineThickness(3.f);

            // spriteOutline2.setFillColor(sf::Color::Transparent);
            // spriteOutline2.setOutlineColor(sf::Color::Green);
            // spriteOutline2.setOutlineThickness(3.f);
    
            // window->draw(spriteOutline);
            // window->draw(spriteOutline2);

            if ((*vec)[j]->getCollision())
            {
                Entity* entity = (*vec)[j];
                CollisionRect coll = *entity->getCollision()->getRect();

                sf::RectangleShape collOutline(coll.getSize());

                collOutline.setOrigin({collOutline.getSize().x / 2.f, collOutline.getSize().y});
                collOutline.setPosition({coll.center().x, coll.bottom()});

                collOutline.setFillColor(sf::Color::Transparent);
                collOutline.setOutlineColor(sf::Color::Blue);
                collOutline.setOutlineThickness(3.f);

                window->draw(collOutline);
            }
        }
    }
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