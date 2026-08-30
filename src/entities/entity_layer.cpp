#include "entity_layer.hpp"
#include "../core/game.hpp"
#include "../utils/utils.hpp"
#include "components/control_component.hpp"
#include "components/inventory_component.hpp"
#include "components/sprite_component.hpp"
#include "entity_systems/entity_chunk_system.hpp"
#include "rect_type.hpp"
#include "states.hpp"
#include "../graphics/asset_manager.hpp"
#include "components/component_data.hpp"
#include "actions/mine_action.hpp"
#include "components/components.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <algorithm>
#include <iterator>

EntityLayer::EntityLayer() {}

void EntityLayer::init(Game* game)
{
    this->game = game;

    IDCounter = 0;

    tManager.entityTemplates["player"] = EntityTemplate();
    tManager.entityTemplates["box"] = EntityTemplate();
    tManager.entityTemplates["item"] = EntityTemplate();

    positionSystem = PositionSystem(game, game->getScene());
    collisionSystem = CollisionSystem(game, game->getScene());
    renderSystem = RenderSystem(game, game->getScene());
    animationSystem = AnimationSystem(game, game->getScene());
    movementSystem = MovementSystem(game, game->getScene());
    actionSystem = ActionSystem(game, game->getScene());
    itemSystem = ItemSystem(game, game->getScene());
    entityChunkSystem = EntityChunkSystem(game, game->getScene());

    auto pt = &tManager.entityTemplates["player"];
    pt->sprite = {game->getAssetManager()->getTexture("dog", "texture_atlases/"), {20, 20}, false, false, {{0, 0}, {0, 0}}, 1.6f, nullptr, game->getAssetManager()->getAnimSet("dog")};
    pt->movement = {1.f, 1.5f};
    pt->control = ControlComponentData();
    pt->state = StateComponentData();
    pt->collision = {{.5f, .5f}, true, RectType::ACTIVE};
    pt->action = {std::make_unique<MineAction>(game, 1, "mine!", 1.f), std::make_unique<Action>(game, "block!", -1.f, 0.f, 4.f, true), game->getSettings()->tile_size * 15};
    pt->inventory = {16, 3, true};

    Entity* e = addEntity(pt, true, {0, 0});
    player = e;

    auto bt = &tManager.entityTemplates["box"];
    bt->sprite = {game->getAssetManager()->getTexture("crate"), {35, 35}, false};
    bt->collision = {{1.f, 1.f}, true, RectType::ACTIVE};

    Entity* b = addEntity(bt, true, {-100, -100});

    auto it = &tManager.entityTemplates["item"];
    it->sprite = {game->getAssetManager()->getTexture("missing"), {1, 1}, true, false};
    it->movement = {0, 0};
    it->collision = {{1, 1}, false, RectType::PASSIVE};
    it->item = {{game->getSettings()->tile_size / 2, game->getSettings()->tile_size / 2}, {0, 0}};

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

Entity* EntityLayer::addEntity(EntityTemplate* t, bool useCustomPosition, sf::Vector2f position)
{
    int ID = getNewID();

    sf::Vector2f usedPosition = position;
    if (t && t->positionData) usedPosition = t->positionData->position;

    entities[ID] = std::make_unique<Entity>(ID, game, usedPosition);
    
    Entity* e = entities[ID].get();
    entityChunkSystem.entityChunkInit(e);

    if (t)
    {
        if (t->sprite) e->addComponent<SpriteComponent>(e, t->sprite->texture, t->sprite->size, t->sprite->sizeIsScale, t->sprite->animation, t->sprite->animSet, t->sprite->usingTexCoords, t->sprite->texCoords, t->sprite->animSpeedMult);
        if (t->movement) e->addComponent<MovementComponent>(e, sf::Vector2f(0, 0), t->movement.value());
        if (t->control) e->addComponent<ControlComponent>(e);
        if (t->state) e->addComponent<StateComponent>(e);
        if (t->collision) e->addComponent<CollisionComponent>(e, e->position, t->collision->size, t->collision->sizeIsScaleOfSprite, t->collision->type);
        if (t->action) e->addComponent<ActionComponent>(e, t->action->mainAction->clone(), t->action->secondaryAction->clone(), t->action->range);
        if (t->item) e->addComponent<ItemComponent>(e, t->item->spawnAreaOffset, t->item->spawnAreaSize);
        if (t->inventory) e->addComponent<InventoryComponent>(e, t->inventory->inventorySize, t->inventory->pickupRange, t->inventory->rangeIsInTiles);
    }

    return entities[ID].get();
}

void EntityLayer::removeEntity(int ID)
{
    if (entities.find(ID) != entities.end())
    {
        if (entities[ID].get() == player) player = nullptr;

        Camera* camera = game->getScene()->getCamera();
        if (entities[ID].get() == camera->getFocus()) camera->removeFocus();

        entities.erase(ID);

        return;
    }

    std::cout << "ERROR removing entity with ID of " << ID << ". That entity was not found.\n";
}

// void EntityLayer::removeAllEntitiesInChunk(int chunkX, int chunkY)
// {
//     float chunkLength = toFloat(game->getSettings()->chunk_size) * game->getSettings()->tile_size;

//     std::vector<int> entitiesToRemove;

//     for (auto& i : entities)
//     {
//         sf::Vector2f entityBottom;
        
//         // if the entity has a sprite, use the bottom of that. If not, just get the entity's position.
//         if (auto s = i.second->getComponent<SpriteComponent>()) entityBottom = {s->sprite.getPosition().x, s->sprite.bottom()};
//         else entityBottom = i.second->position.getPosition();

//         if (i.first == 1) std::cout << "here\n";
//         if (worldToChunkPosition(game, entityBottom) == sf::Vector2i(chunkX, chunkY))
//         {
//             entitiesToRemove.push_back(i.second->ID);
//         }
//     }

//     for (int i = 0; i < entitiesToRemove.size(); i++)
//     {
//         removeEntity(entitiesToRemove[i]);
//     }
// }

Entity* EntityLayer::getEntity(int ID)
{
    if (entities.find(ID) != entities.end())
    {
        return entities[ID].get();
    }

    return nullptr;
}

Entity* EntityLayer::getEntityAtPos(sf::Vector2f position, bool includePlayer)
{
    std::vector<Entity*> entitiesWithSprite = getEntitiesWithComponent<SpriteComponent>();

    std::sort(entitiesWithSprite.begin(), entitiesWithSprite.end(), [](Entity* a, Entity* b){
        return a->getComponent<SpriteComponent>()->sprite.bottom() < b->getComponent<SpriteComponent>()->sprite.bottom();
    });

    for (auto itr = entitiesWithSprite.rbegin(); itr != entitiesWithSprite.rend(); itr++)
    {
        if (!includePlayer && (*itr) == player) continue;

        if (auto s = (*itr)->getComponent<SpriteComponent>())
        {
            if (pointRectCollide(position, s->sprite.sprite->getGlobalBounds()))
            {
                return *itr;
            }
        }
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
        sf::Vector2f entityBottom = i.second->position.getPosition();
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

std::map<int, std::unique_ptr<Entity>>* EntityLayer::getAllEntities()
{
    return &entities;
}

void EntityLayer::tick()
{
    positionSystem.tick();

    movementSystem.tick();

    itemSystem.tick();

    collisionSystem.tick();

    entityChunkSystem.tick();

    // TEMP, TODO: player detection or something
    if (player == nullptr)
    {
        for (auto& e : entities)
        {
            if (e.second->getComponent<ControlComponent>())
            {
                player = e.second.get();
                break;
            }
        }
    }
}

void EntityLayer::update(float dt)
{
    actionSystem.update(dt);

    animationSystem.update(dt);

    renderSystem.update(dt);
}

void EntityLayer::draw(bool debug)
{
    renderSystem.draw(debug);
}
