#include "entity_layer.hpp"
#include "../core/game.hpp"
#include "../utils/utils.hpp"
#include "components/collision_component.hpp"
#include "rect_type.hpp"
#include "states.hpp"
#include "../graphics/asset_manager.hpp"
#include "components/component_data.hpp"
#include "components/state_component.hpp"
#include "components/control_component.hpp"
#include "components/movement_component.hpp"
#include "components/action_component.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <algorithm>

EntityLayer::EntityLayer() {}

void EntityLayer::init(Game* game)
{
    this->game = game;

    IDCounter = 0;

    tManager.entityTemplates["player"] = EntityTemplate();
    tManager.entityTemplates["box"] = EntityTemplate();

    auto pt = &tManager.entityTemplates["player"];
    pt->sprite = {game->getAssetManager()->getTexture("dog", "texture_atlases/"), {20, 20}, false, false, {{0, 0}, {0, 0}}, 2.f, nullptr, game->getAssetManager()->getAnimSet("dog")};
    pt->movement = {2.f, 1.5f};
    pt->control = ControlComponentData();
    pt->state = StateComponentData();
    pt->collision = {{.5f, .5f}, true, RectType::ACTIVE};
    pt->action = {std::make_unique<Action>("attack!", 1.f, 0.5f, false), std::make_unique<Action>("block!", -1.f, 4.f, true), game->getSettings()->tile_size * 5};

    Entity* e = addEntity({0, 0}, pt);
    this->player = e;

    auto bt = &tManager.entityTemplates["box"];
    bt->sprite = {game->getAssetManager()->getTexture("crate"), {35, 35}, false, false, {{0, 0}, {0, 0}}, 1, nullptr, nullptr};
    bt->collision = {{1.f, 1.f}, true, RectType::PASSIVE};

    Entity* b = addEntity({-100, -100}, bt);

    // // PERFORMANCE TEST
    // for (int y = 0; y < 20; y++)
    // {
    //     for (int x = 0; x < 50; x++)
    //     {
    //         addEntity({static_cast<float>(-600 + x * bt->sprite.size.x), static_cast<float>(50 + y * bt->sprite.size.y * 2)}, bt);
    //     }
    // }
    // // // // // // // /

    // AssetManager* assetManager = game->getAssetManager();

    // addEntity(getNewID(), {475, 475});
    // addEntity(getNewID(), {0, 0});
    // addEntity(getNewID(), {900, 900});
    // addEntity(getNewID(), {100, 500});
    // addEntity(getNewID(), {0, 0});
    // addEntity(getNewID(), {-400, 100});
    // addEntity(getNewID(), {-400, 250});
    // addEntity(getNewID(), {-400, 400});
    // addEntity(getNewID(), {-400, 550});
    // addEntity(getNewID(), {-400, 700});
    // addEntity(getNewID(), {-200, 700});
    // addEntity(getNewID(), {0, 1000});
    // addEntity(getNewID(), {200, 1000});
    // addEntity(getNewID(), {400, 1000});
    // addEntity(getNewID(), {600, 1000});
    // addEntity(getNewID(), {0, 1200});
    // addEntity(getNewID(), {0, 1400});
    // addEntity(getNewID(), {0, 1600});

    // giveEntitySprite(0, assetManager->getTexture("pixel"), {50, 50}, -1);

    // playerID = 1;
    // giveEntitySprite(1, assetManager->getTexture("IDLE_smaller"), {24 * 10, 21 * 10});
    // getEntity(1)->getSprite()->giveAnimationSet(assetManager->getAnimSet("player"));
    // giveEntityMotion(1, 1.f, true);
    // giveEntityCollision(1, "player", ACTIVE, {}, {0, 0.2}, {0.3, 0.3});

    // giveEntitySprite(2, assetManager->getTexture("dr bee"), {200, 200});
    // giveEntityCollision(2, "enemy", STATIC);

    // giveEntitySprite(3, assetManager->getTexture("bush"), {200, 120});
    // giveEntityCollision(3, "obstacle", STATIC, {}, {0, 0.2f}, {.7f, .6f});

    // giveEntitySprite(4, assetManager->getTexture("IDLE_smaller"), {24 * 11, 21 * 11});
    // getEntity(4)->getSprite()->giveAnimation(assetManager->getAnimation("knight_idle"));

    // giveEntitySprite(5, assetManager->getTexture("pixel"), {100, 100}, -2);
    // getEntity(5)->getSprite()->giveAnimation(assetManager->getAnimation("dot_left"));

    // giveEntitySprite(6, assetManager->getTexture("pixel"), {100, 100}, -2);
    // getEntity(6)->getSprite()->giveAnimation(assetManager->getAnimation("dot_right"));

    // giveEntitySprite(7, assetManager->getTexture("pixel"), {100, 100}, -2);
    // getEntity(7)->getSprite()->giveAnimation(assetManager->getAnimation("dot_up"));

    // giveEntitySprite(8, assetManager->getTexture("pixel"), {100, 100}, -2);
    // getEntity(8)->getSprite()->giveAnimation(assetManager->getAnimation("dot_down"));

    // giveEntitySprite(9, assetManager->getTexture("pixel"), {100, 100}, -2);
    // getEntity(9)->getSprite()->giveAnimation(assetManager->getAnimation("dot_idle"));

    // giveEntitySprite(10, assetManager->getTexture("crate"), {150, 150});
    // giveEntityMotion(10, 1.f);
    // giveEntityCollision(10, "crate", MOVABLE);

    // giveEntitySprite(11, assetManager->getTexture("crate"), {200.f, 200.f}, -1);
    // giveEntityCollision(11, "wall", STATIC);

    // giveEntitySprite(12, assetManager->getTexture("crate"), {200.f, 200.f}, -1);
    // giveEntityCollision(12, "wall", STATIC);

    // giveEntitySprite(13, assetManager->getTexture("crate"), {200.f, 200.f}, -1);
    // giveEntityCollision(13, "wall", STATIC);

    // giveEntitySprite(14, assetManager->getTexture("crate"), {200.f, 200.f}, -1);
    // giveEntityCollision(14, "wall", STATIC);

    // giveEntitySprite(15, assetManager->getTexture("crate"), {200.f, 200.f}, -1);
    // giveEntityCollision(15, "wall", STATIC);

    // giveEntitySprite(16, assetManager->getTexture("crate"), {200.f, 200.f}, -1);
    // giveEntityCollision(16, "wall", STATIC);

    // giveEntitySprite(17, assetManager->getTexture("crate"), {200.f, 200.f}, -1);
    // giveEntityCollision(17, "wall", STATIC);

}

int EntityLayer::getNewID()
{
    IDCounter++;

    return IDCounter - 1;
}

Entity* EntityLayer::addEntity(sf::Vector2f position, EntityTemplate* t)
{
    int ID = getNewID();

    entities[ID] = std::make_unique<Entity>(game, ID, position);
    
    Entity* e = entities[ID].get();

    if (t)
    {
        e->spriteInit(t->sprite.texture, t->sprite.size, t->sprite.sizeIsScale, t->sprite.usingTexCoords, t->sprite.texCoords, t->sprite.animSpeedMult);

        if (t->sprite.animation)
        {
            e->getSprite()->animation = std::make_unique<Animation>(*t->sprite.animation);
        }
        else if (t->sprite.animSet)
        {
            e->getSprite()->animSet = std::make_unique<AnimationSet>(*t->sprite.animSet);
        }

        if (t->movement) e->addComponent<MovementComponent>(e, sf::Vector2f(0, 0), t->movement.value());
        if (t->control) e->addComponent<ControlComponent>(e);
        if (t->state) e->addComponent<StateComponent>(e);
        if (t->collision) e->addComponent<CollisionComponent>(e, *e->getPositionVar(), t->collision->size, t->collision->sizeIsScaleOfSprite, t->collision->type);
        if (t->action) e->addComponent<ActionComponent>(e, std::move(t->action->mainAction), std::move(t->action->secondaryAction), t->action->range);
    }

    return entities[ID].get();
}

void EntityLayer::removeEntity(int ID)
{
    if (entities.find(ID) != entities.end())
    {
        entities.erase(ID);

        return;
    }

    std::cout << "ERROR removing entity with ID of " << ID << ". That entity was not found.\n";
}

void EntityLayer::removeAllEntitiesInChunk(int chunkX, int chunkY)
{
    float chunkLength = toFloat(game->getSettings()->chunk_size) * game->getSettings()->tile_size;

    std::vector<int> entitiesToRemove;

    for (auto& i : entities)
    {
        sf::Vector2f entityBottom = {i.second->getSprite()->getPosition().x, i.second->getSprite()->bottom()};

        if (worldToChunkPosition(game, entityBottom) == sf::Vector2i(chunkX, chunkY))
        {
            entitiesToRemove.push_back(i.second->getID());
        }
    }

    for (int i = 0; i < entitiesToRemove.size(); i++)
    {
        removeEntity(entitiesToRemove[i]);
    }
}

Entity* EntityLayer::getEntity(int ID)
{
    if (entities.find(ID) != entities.end())
    {
        return entities[ID].get();
    }

    return nullptr;
}

std::vector<Entity*> EntityLayer::getEntitiesInChunkArea(int chunkX, int chunkY, int chunkRadius)
{
    std::vector<Entity*> entitiesWithin;

    float chunkLength = toFloat(game->getSettings()->chunk_size) * game->getSettings()->tile_size;

    sf::Vector2i chunkMin = {chunkX - chunkRadius, chunkY - chunkRadius};
    sf::Vector2i chunkMax = {chunkX + chunkRadius, chunkY + chunkRadius};

    for (auto& i : entities)
    {
        sf::Vector2f entityBottom = i.second->getPosition();
        int entityChunkBottomX = toInt(std::floor(entityBottom.x / chunkLength));
        int entityChunkBottomY = toInt(std::floor(entityBottom.y / chunkLength));

        if (entityChunkBottomX >= chunkMin.x && entityChunkBottomX <= chunkMax.x)
        {
            if (entityChunkBottomY >= chunkMin.y && entityChunkBottomY <= chunkMax.y)
            {
                entitiesWithin.push_back(i.second.get());
            }
        }
    }

    return entitiesWithin;
}

std::vector<Entity*> EntityLayer::getEntitiesInChunkArea(sf::Vector2f position, int chunkRadius)
{
    float chunkLength = toFloat(game->getSettings()->chunk_size) * game->getSettings()->tile_size;

    int chunkX = toInt(std::floor(position.x / chunkLength));
    int chunkY = toInt(std::floor(position.y / chunkLength));

    return getEntitiesInChunkArea(chunkX, chunkY, chunkRadius);   
}

void EntityLayer::tick()
{
    for (auto& i : entities)
    {
        Chunk* entityChunk = game->getScene()->getChunkLayer()->getChunk(worldToChunkPosition(game, i.second->getPosition()));

        if (entityChunk && entityChunk->state == ChunkState::ACTIVE)
        {
            i.second->tick();
        }
    }
}

void EntityLayer::update(float dt)
{
    for (auto& i : entities)
    {
        Chunk* entityChunk = game->getScene()->getChunkLayer()->getChunk(worldToChunkPosition(game, i.second->getPosition()));

        if (entityChunk && entityChunk->state == ChunkState::ACTIVE)
        {
            i.second->update(dt);
        }
    }
}

void EntityLayer::draw(bool debug)
{
    std::vector<int> outOfBoundsEntities;

    for (auto& i : entities)
    {
        Chunk* entityChunk = game->getScene()->getChunkLayer()->getChunk(worldToChunkPosition(game, i.second->getPosition()));

        if (entityChunk)
        {
            if (entityChunk->state == ChunkState::ACTIVE)
            {
                Sprite* entitySprite = i.second->getSprite();
    
                if (isOnScreen(game, {entitySprite->left(), entitySprite->top()}, entitySprite->getSize()))
                {
                    i.second->draw(game->getWindow()->getWindow());

                    if (debug)
                    {
                        if (auto c = i.second->getComponent<CollisionComponent>())
                        {
                            sf::RectangleShape rect(c->rect.size);
                            rect.setPosition({c->rect.left(), c->rect.top()});
                            rect.setFillColor(sf::Color::Transparent);
                            rect.setOutlineColor(sf::Color::Red);
                            rect.setOutlineThickness(.5f);
                            game->getWindow()->draw(rect);
                        }
                    }
                }
            }
        }
        else
        {
            // TODO: When chunk saving is implemented, entities unloaded here should be saved.
            outOfBoundsEntities.push_back(i.second->getID());
        }
    }

    for (auto i : outOfBoundsEntities) removeEntity(i);
}