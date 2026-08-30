#pragma once

#include <unordered_map>
#include <vector>
#include <map>
#include <memory>

#include "../core/common.hpp"
#include "entity.hpp"
#include "entity_template.hpp"
#include "template_manager.hpp"
#include "entity_systems/collision_system.hpp"
#include "entity_systems/render_system.hpp"
#include "entity_systems/animation_system.hpp"
#include "entity_systems/movement_system.hpp"
#include "entity_systems/action_system.hpp"
#include "entity_systems/position_system.hpp"
#include "entity_systems/item_system.hpp"
#include "entity_systems/entity_chunk_system.hpp"

class Game;

class EntityLayer
{
public:
    EntityLayer();

    void init(Game* game);

    int getNewID();    

    Entity* addEntity(EntityTemplate* t = nullptr, bool useCustomPosition = false, sf::Vector2f position = {0, 0});

    void removeEntity(int ID);

    // void removeAllEntitiesInChunk(int chunkX, int chunkY);

    Entity* getEntity(int ID);

    Entity* getEntityAtPos(sf::Vector2f position, bool includePlayer);

    // chunkRadius controls the distance to scan for entities.
    // The default of 0 only scans in the chunk position given.
    std::vector<Entity*> getEntitiesInChunkArea(int chunkX, int chunkY, int chunkRadius = 0);
    std::vector<Entity*> getEntitiesInChunkArea(sf::Vector2f position, int chunkRadius = 0);

    template<typename A>
    std::vector<Entity*> getEntitiesWithComponent()
    {
        std::vector<Entity*> entitiesWithRequiredComponent;

        for (auto& e : entities)
        {
            if (e.second->getComponent<A>())
            {
                entitiesWithRequiredComponent.emplace_back(e.second.get());
            }
        }

        return entitiesWithRequiredComponent;
    }

    template<typename A, typename B>
    std::vector<Entity*> getEntitiesWithComponents()
    {
        std::vector<Entity*> entitiesWithRequiredComponents;

        for (auto& e : entities)
        {
            if (
                e.second->getComponent<A>() &&
                e.second->getComponent<B>()
            )
            {
                entitiesWithRequiredComponents.emplace_back(e.second.get());
            }
        }

        return entitiesWithRequiredComponents;
    }

    template<typename A, typename B, typename C>
    std::vector<Entity*> getEntitiesWithComponents()
    {
        std::vector<Entity*> entitiesWithRequiredComponents;

        for (auto& e : entities)
        {
            if (
                e.second->getComponent<A>() &&
                e.second->getComponent<B>() &&
                e.second->getComponent<C>()
            )
            {
                entitiesWithRequiredComponents.emplace_back(e.second.get());
            }
        }

        return entitiesWithRequiredComponents;
    }

    template<typename A, typename B, typename C, typename D>
    std::vector<Entity*> getEntitiesWithComponents()
    {
        std::vector<Entity*> entitiesWithRequiredComponents;

        for (auto& e : entities)
        {
            if (
                e.second->getComponent<A>() &&
                e.second->getComponent<B>() &&
                e.second->getComponent<C>() &&
                e.second->getComponent<D>()
            )
            {
                entitiesWithRequiredComponents.emplace_back(e.second.get());
            }
        }

        return entitiesWithRequiredComponents;
    }

    std::map<int, std::unique_ptr<Entity>>* getAllEntities();

    void tick();

    void update(float dt);

    void draw(bool debug);

    Entity* player;
    
    TemplateManager tManager;
private:
    Game* game;

    int IDCounter;

    PositionSystem positionSystem;
    CollisionSystem collisionSystem;
    RenderSystem renderSystem;
    AnimationSystem animationSystem;
    MovementSystem movementSystem;
    ActionSystem actionSystem;
    ItemSystem itemSystem;
    EntityChunkSystem entityChunkSystem;
    
    std::map<int, std::unique_ptr<Entity>> entities;
};