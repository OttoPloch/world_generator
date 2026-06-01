#include "collision_system.hpp"
#include "../../core/game.hpp"
#include "../entity_layer.hpp"
#include "../components/components.hpp"
#include <algorithm>

CollisionSystem::CollisionSystem() {}

CollisionSystem::CollisionSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void CollisionSystem::update(float dt)
{
    std::vector<Entity*> validEntities = entityLayer->getEntitiesWithComponents<MovementComponent, CollisionComponent>();

    for (auto e : validEntities)
    {
        auto movement = e->getComponent<MovementComponent>();
        auto collision = e->getComponent<CollisionComponent>();
        
        findAndResolveCollisions(e, movement, collision);
    }
}

void CollisionSystem::findAndResolveCollisions(Entity* e, MovementComponent* m, CollisionComponent* c)
{
    CollisionRect& rect = c->rect;

    if (rect.type == RectType::ACTIVE)
    {
        // TILE COLLISION
        std::array<Chunk*, 9> nearbyChunks = scene->getChunkLayer()->getNearbyChunks(e->getComponent<PositionComponent>()->position.getPosition());
        // Chunk, position, z-value
        std::vector<std::pair<Chunk*, std::pair<sf::Vector2i, int>>> nearbyTilesWithColliders;

        // getting the colliders of every nearby tile
        for (int i = 0; i < nearbyChunks.size(); i++)
        {
            if (nearbyChunks[i])
            {
                std::vector<Tile*>* currChunkTWithC = &nearbyChunks[i]->tilesWithColliders;

                for (int j = 0; j < currChunkTWithC->size(); j++)
                {
                    Tile* currTile = (*currChunkTWithC)[j];
    
                    nearbyTilesWithColliders.push_back({nearbyChunks[i], {currTile->localPosition, currTile->z}});
                }
            }
        }

        sf::Vector2f contactPoint;
        sf::Vector2f contactNormal;
        float contactTime;

        if (nearbyTilesWithColliders.size() > 0)
        {
            // <colliding tile, contact time>
            std::vector<std::pair<CollisionRect, float>> collidingTiles;

            for (auto t : nearbyTilesWithColliders)
            {
                Tile* tile = t.first->getTile(t.second.first.x, t.second.first.y, false, t.second.second);
                sf::FloatRect tileRect = t.first->getTileRect(t.second.first, t.second.second);
                GamePosition tilePos(game, tileRect.position);
                CollisionRect tileCollRect(tilePos, tileRect.size, RectType::STATIC);

                if (dynamicRectRectCollide(&rect, m->velocity, &tileCollRect, contactPoint, contactNormal, contactTime))
                {
                    collidingTiles.emplace_back(tileCollRect, contactTime);
                }
            }

            std::sort(collidingTiles.begin(), collidingTiles.end(), [](const std::pair<CollisionRect, float>& a, const std::pair<CollisionRect, float>& b)
            {
                return a.second < b.second;
            });
    
            for (auto t : collidingTiles)
            {
                if (dynamicRectRectCollide(&rect, m->velocity, &t.first, contactPoint, contactNormal, contactTime))
                {
                    m->velocity += sf::Vector2f(std::abs(m->velocity.x) * contactNormal.x, std::abs(m->velocity.y) * contactNormal.y) * (1.f - contactTime);
                    m->velocity += {contactNormal.x * .001f, contactNormal.y * .001f};
                }
            }
        }

        // ENTITY COLLISION
        std::vector<Entity*> entities = entityLayer->getEntitiesInChunkArea(rect.position.getPosition(), 1);
    
        // <rect, contact time>
        std::vector<std::pair<CollisionRect*, float>> z;

        for (auto i : entities)
        {
            if (i->ID == e->ID) continue;
    
            if (auto c = i->getComponent<CollisionComponent>())
            {
                CollisionRect* other = &c->rect;

                if (dynamicRectRectCollide(&rect, m->velocity, other, contactPoint, contactNormal, contactTime))
                {
                    z.emplace_back(&c->rect, contactTime);
                }
            }
        }

        if (z.size() > 0)
        {
            std::sort(z.begin(), z.end(), [](const std::pair<CollisionRect*, float>& a, const std::pair<CollisionRect*, float>& b)
            {
                return a.second < b.second;
            });

            for (auto i : z)
            {
                if (dynamicRectRectCollide(&rect, m->velocity, i.first, contactPoint, contactNormal, contactTime))
                {
                    m->velocity += sf::Vector2f(std::abs(m->velocity.x) * contactNormal.x, std::abs(m->velocity.y) * contactNormal.y) * (1.f - contactTime);
                    m->velocity += {contactNormal.x * .001f, contactNormal.y * .001f};
                }
            }
        }
    }
}