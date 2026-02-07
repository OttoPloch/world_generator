#include "entity_layer.hpp"
#include "game.hpp"
#include "rect_types.hpp"

#include <algorithm>

EntityLayer::EntityLayer() {}

void EntityLayer::init(Game* game, std::vector<std::unique_ptr<Entity>> entities)
{
    this->game = game;

    IDCounter = 0;
    
    this->entities.resize(entities.size());

    for (int i = 0; i < entities.size(); i++)
    {
        this->entities[i] = std::move(entities[i]);
    }

    if (entities.size() > 0)
    {
        for (int i = 0; i < entities.size(); i++)
        {
            if (entities[i]->getSprite())
            {
                entitiesZMap[entities[i]->getSprite()->getZ()].push_back(entities[i]->getID());
            }
        }
    }

    AssetManager* assetManager = game->getAssetManager();

    addEntity(getNewID(), {475, 475});
    addEntity(getNewID(), {0, 0});
    addEntity(getNewID(), {900, 900});
    addEntity(getNewID(), {100, 500});
    addEntity(getNewID(), {0, 0});
    addEntity(getNewID(), {-400, 100});
    addEntity(getNewID(), {-400, 250});
    addEntity(getNewID(), {-400, 400});
    addEntity(getNewID(), {-400, 550});
    addEntity(getNewID(), {-400, 700});
    addEntity(getNewID(), {-200, 700});
    addEntity(getNewID(), {0, 1000});
    addEntity(getNewID(), {200, 1000});
    addEntity(getNewID(), {400, 1000});
    addEntity(getNewID(), {600, 1000});

    giveEntitySprite(0, assetManager->getTexture("pixel"), {50, 50}, -1);

    giveEntitySprite(1, assetManager->getTexture("IDLE_smaller"), {24 * 10, 21 * 10});
    getEntity(1)->getSprite()->giveAnimationSet(assetManager->getAnimSet("player"));
    giveEntityMotion(1, 1.f, true);
    giveEntityCollision(1, "player", ACTIVE, {}, {0, 0.2}, {0.3, 0.3});

    giveEntitySprite(2, assetManager->getTexture("dr bee"), {200, 200});
    giveEntityCollision(2, "enemy", STATIC);

    giveEntitySprite(3, assetManager->getTexture("bush"), {200, 120});
    giveEntityCollision(3, "obstacle", STATIC, {}, {0, 0.2f}, {.7f, .6f});

    giveEntitySprite(4, assetManager->getTexture("IDLE_smaller"), {24 * 11, 21 * 11});
    getEntity(4)->getSprite()->giveAnimation(assetManager->getAnimation("knight_idle"));

    giveEntitySprite(5, assetManager->getTexture("pixel"), {100, 100}, -2);
    getEntity(5)->getSprite()->giveAnimation(assetManager->getAnimation("dot_left"));

    giveEntitySprite(6, assetManager->getTexture("pixel"), {100, 100}, -2);
    getEntity(6)->getSprite()->giveAnimation(assetManager->getAnimation("dot_right"));

    giveEntitySprite(7, assetManager->getTexture("pixel"), {100, 100}, -2);
    getEntity(7)->getSprite()->giveAnimation(assetManager->getAnimation("dot_up"));

    giveEntitySprite(8, assetManager->getTexture("pixel"), {100, 100}, -2);
    getEntity(8)->getSprite()->giveAnimation(assetManager->getAnimation("dot_down"));

    giveEntitySprite(9, assetManager->getTexture("pixel"), {100, 100}, -2);
    getEntity(9)->getSprite()->giveAnimation(assetManager->getAnimation("dot_idle"));

    giveEntitySprite(10, assetManager->getTexture("crate"), {150, 150});
    giveEntityMotion(10, 1.f);
    giveEntityCollision(10, "crate", MOVABLE);

    giveEntitySprite(11, assetManager->getTexture("crate"), {200.f, 200.f}, -1);
    giveEntityCollision(11, "wall", STATIC);

    giveEntitySprite(12, assetManager->getTexture("crate"), {200.f, 200.f}, -1);
    giveEntityCollision(12, "wall", STATIC);

    giveEntitySprite(13, assetManager->getTexture("crate"), {200.f, 200.f}, -1);
    giveEntityCollision(13, "wall", STATIC);

    giveEntitySprite(14, assetManager->getTexture("crate"), {200.f, 200.f}, -1);
    giveEntityCollision(14, "wall", STATIC);

}

int EntityLayer::getNewID()
{
    IDCounter++;

    return IDCounter - 1;
}

void EntityLayer::addEntity(int ID, sf::Vector2f position)
{
    entities.push_back(std::make_unique<Entity>(game, ID, position));
}

void EntityLayer::removeEntity(int ID)
{
    if (entities.size() > 0)
    {
        for (auto it = entities.begin(); it != entities.end(); it++)
        {
            if ((*it)->getID() == ID)
            {
                removeFromZMap(ID);

                entities.erase(it);

                return;
            }
        }

        std::cout << "ERROR removing entity with ID of " << ID << ". That entity was not in the list.\n";
    }
}

Entity* EntityLayer::getEntity(int ID)
{
    if (entities.size() > 0)
    {
        for (int i = 0; i < entities.size(); i++)
        {
            if (entities[i]->getID() == ID)
            {
                return entities[i].get();
            }
        }
    }

    return nullptr;
}

void EntityLayer::removeFromZMap(int ID)
{
    Entity* entity = getEntity(ID);

    if (entity->getSprite())
    {
        std::vector<int>* zMapVector = &entitiesZMap[entity->getSprite()->getZ()];

        for (auto it = zMapVector->begin(); it != zMapVector->end(); it++)
        {
            if (*it == ID)
            {
                zMapVector->erase(it);

                return;
            }
        }
    }
}

void EntityLayer::addToZMap(int ID)
{
    Entity* entity = getEntity(ID);

    if (entity->getSprite())
    {
        entitiesZMap[entity->getSprite()->getZ()].push_back(ID);
    }
}

void EntityLayer::giveEntitySprite(int ID, sf::Texture* texture, sf::Vector2f size, int z, bool centerOrigin)
{
    Entity* entity = getEntity(ID);

    entity->giveSprite(texture, size, z, centerOrigin);
    addToZMap(ID);
}

void EntityLayer::giveEntityMotion(int ID, float mass, bool controlling)
{
    Entity* entity = getEntity(ID);

    entity->giveMotion(mass, controlling);
}

void EntityLayer::giveEntityCollision(int ID, std::string name, int rectType, std::vector<std::string> blacklist, sf::Vector2f offsetFraction, sf::Vector2f size, bool sizeIsFraction)
{
    Entity* entity = getEntity(ID);

    entity->giveCollision(&entities, name, rectType, blacklist, offsetFraction, size, sizeIsFraction);
}

void EntityLayer::tick()
{
    if (entities.size() > 0)
    {
        for (int i = 0; i < entities.size(); i++)
        {
            entities[i]->tick();
        }
    }
}

void EntityLayer::update(float dt)
{
    if (entities.size() > 0)
    {
        for (int i = 0; i < entities.size(); i++)
        {
            entities[i]->update(dt);
        }
    }
}

void EntityLayer::draw()
{
    for (auto i : entitiesZMap)
    {
        std::vector<int>* vec = &i.second;

        if (vec->size() > 0)
        {
            std::vector<Entity*> entityVec;

            for (int j = 0; j < vec->size(); j++)
            {
                entityVec.push_back(getEntity((*vec)[j]));
            }

            sortEntitiesByY(&entityVec, 0, entityVec.size() - 1);
    
            for (int j = 0; j < entityVec.size(); j++)
            {
                entityVec[j]->draw(game->getWindow()->getWindow());
    
                // Entity* entity = entityVec[j];
                // Sprite* sprite = entity->getSprite();
                // sf::Sprite sprite2 = sprite->getSprite();
                // sf::Vector2f size = toV2F(sprite2.getTextureRect().size);
                // sf::Vector2f size2 = sprite->getSize();
    
                // sf::RectangleShape spriteOutline(size);
                // sf::RectangleShape spriteOutline2(size2);
    
                // spriteOutline.setOrigin({size.x / 2.f, size.y / 2.f});
                // spriteOutline.setPosition(entityVec[j]->getPosition());
                
                // spriteOutline2.setOrigin({size2.x / 2.f, size2.y / 2.f});
                // spriteOutline2.setPosition(entityVec[j]->getPosition());
        
                // spriteOutline.setFillColor(sf::Color::Transparent);
                // spriteOutline.setOutlineColor(sf::Color::Red);
                // spriteOutline.setOutlineThickness(3.f);
    
                // spriteOutline2.setFillColor(sf::Color::Transparent);
                // spriteOutline2.setOutlineColor(sf::Color::Green);
                // spriteOutline2.setOutlineThickness(3.f);
                
                // game->getWindow()->draw(spriteOutline);
                // game->getWindow()->draw(spriteOutline2);
    
                // if (entityVec[j]->getCollision())
                // {
                //     Entity* entity = entityVec[j];
                //     CollisionRect coll = *entity->getCollision()->getRect();
    
                //     sf::RectangleShape collOutline(coll.getSize());
    
                //     collOutline.setOrigin({collOutline.getSize().x / 2.f, collOutline.getSize().y});
                //     collOutline.setPosition({coll.center().x, coll.bottom()});
    
                //     collOutline.setFillColor(sf::Color::Transparent);
                //     collOutline.setOutlineColor(sf::Color::Blue);
                //     collOutline.setOutlineThickness(3.f);
    
                //     game->getWindow()->draw(collOutline);
                // }
            }
        }
    }
}