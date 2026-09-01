#include "collision_system.hpp"
#include "../../core/game.hpp"
#include "../entity_layer.hpp"
#include "../components/components.hpp"
#include <algorithm>

CollisionSystem::CollisionSystem() {}

CollisionSystem::CollisionSystem(Game* game, Scene* scene) : game(game), scene(scene), entityLayer(scene->getEntityLayer()) {}

void CollisionSystem::tick()
{
    std::vector<int> noLongerValidEntities;

    for (auto entity : validEntities)
    {
        auto entityMovementComponent = entity->getComponent<MovementComponent>();
        auto entityCollisionComponent = entity->getComponent<CollisionComponent>();

        if (!entityMovementComponent || !entityCollisionComponent)
        {
            noLongerValidEntities.emplace_back(entity->ID);
            continue;
        }

        findAndResolveCollisions(entity, entityMovementComponent, entityCollisionComponent);
    }

    removeAllEntityIDsInVec(validEntities, noLongerValidEntities);
}

void CollisionSystem::refactorEntityCache()
{
    validEntities = entityLayer->getEntitiesWithComponents<MovementComponent, CollisionComponent>();
}

void CollisionSystem::findAndResolveCollisions(Entity* entity, MovementComponent* entityMovementComponent, CollisionComponent* entityCollisionComponent)
{
    CollisionRect& rect = entityCollisionComponent->rect;

    tileCollision(entity, rect, entityMovementComponent);

    entityCollision(entity, rect, entityMovementComponent);
}

void CollisionSystem::tileCollision(Entity* entity, CollisionRect& rect, MovementComponent* entityMovementComponent)
{
    sf::Vector2f contactPoint;
    sf::Vector2f contactNormal;
    float contactTime;


    std::array<Chunk*, 9> nearbyChunks = scene->getChunkLayer()->getNearbyChunks(entity->position.getPosition());
    // Chunk, tile position, z-value
    std::vector<std::pair<Chunk*, std::pair<sf::Vector2i, int>>> nearbyTilesWithColliders;

    // getting the colliders of every nearby tile
    for (int i = 0; i < nearbyChunks.size(); i++)
    {
        if (nearbyChunks[i])
        {
            std::vector<Tile*>* currChunkTilesWithColliders = &nearbyChunks[i]->tilesWithColliders;

            for (int j = 0; j < currChunkTilesWithColliders->size(); j++)
            {
                Tile* currTile = (*currChunkTilesWithColliders)[j];

                nearbyTilesWithColliders.push_back({nearbyChunks[i], {currTile->localPosition, currTile->z}});
            }
        }
    }

    if (nearbyTilesWithColliders.size() > 0)
    {
        // <colliding tile, contact time>
        std::vector<std::pair<CollisionRect, float>> collidingTiles;

        for (auto t : nearbyTilesWithColliders)
        {
            Tile* tile = t.first->getTile(t.second.first, false, t.second.second);
            sf::FloatRect tileRect = t.first->getTileRect(t.second.first, t.second.second);
            GamePosition tilePos(game, tileRect.position);
            CollisionRect tileCollRect(tilePos, tileRect.size, RectType::STATIC);

            if (dynamicRectRectCollide(&rect, entityMovementComponent->velocity, &tileCollRect, contactPoint, contactNormal, contactTime))
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
            if (dynamicRectRectCollide(&rect, entityMovementComponent->velocity, &t.first, contactPoint, contactNormal, contactTime))
            {
                entityMovementComponent->velocity += sf::Vector2f(std::abs(entityMovementComponent->velocity.x) * contactNormal.x, std::abs(entityMovementComponent->velocity.y) * contactNormal.y) * (1.f - contactTime);
                entityMovementComponent->velocity += {contactNormal.x * .001f, contactNormal.y * .001f};
            }
        }
    }
}

void CollisionSystem::entityCollision(Entity* entity, CollisionRect& rect, MovementComponent* entityMovementComponent)
{
    sf::Vector2f contactPoint;
    sf::Vector2f contactNormal;
    float contactTime;


    std::vector<Entity*> nearbyEntities = entityLayer->getEntitiesInChunkArea(rect.position.getPosition(), 1);

    // <rect, contact time>
    std::vector<std::pair<CollisionRect*, float>> z;

    for (auto curr : nearbyEntities)
    {
        if (curr->ID == entity->ID) continue;
        
        if (auto currCollisionComponent = curr->getComponent<CollisionComponent>())
        {
            if (rect.type != RectType::PASSIVE && currCollisionComponent->rect.type == RectType::PASSIVE) continue;
            if (rect.type == RectType::PASSIVE && currCollisionComponent->rect.type == RectType::PASSIVE) continue;
            if (entity->getComponent<ItemComponent>() && curr->getComponent<InventoryComponent>()) continue;

            CollisionRect* other = &currCollisionComponent->rect;

            if (dynamicRectRectCollide(&rect, entityMovementComponent->velocity, other, contactPoint, contactNormal, contactTime))
            {
                z.emplace_back(&currCollisionComponent->rect, contactTime);
            }
        }
    }

    if (z.size() > 0)
    {
        std::sort(z.begin(), z.end(), [](const std::pair<CollisionRect*, float>& a, const std::pair<CollisionRect*, float>& b)
        {
            return a.second < b.second;
        });

        for (auto pair : z)
        {
            if (dynamicRectRectCollide(&rect, entityMovementComponent->velocity, pair.first, contactPoint, contactNormal, contactTime))
            {
                entityMovementComponent->velocity += sf::Vector2f(std::abs(entityMovementComponent->velocity.x) * contactNormal.x, std::abs(entityMovementComponent->velocity.y) * contactNormal.y) * (1.f - contactTime);
                entityMovementComponent->velocity += {contactNormal.x * .001f, contactNormal.y * .001f};
            }
        }
    }
}