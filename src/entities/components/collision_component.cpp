#include "collision_component.hpp"
#include "../entity.hpp"
#include "../../core/game.hpp"
#include "component_data.hpp"
#include "entity_component.hpp"
#include "movement_component.hpp"
#include <cstddef>
#include <algorithm>

CollisionComponent::CollisionComponent(Entity* myEntity, WorldPosition position, sf::Vector2f size, bool sizeIsScaleOfSprite, RectType type) : EntityComponent(myEntity)
{
    sf::Vector2f adjustedSize = size;

    if (sizeIsScaleOfSprite)
    {
        sf::Vector2f spriteSize = myEntity->getSprite()->getSize();

        adjustedSize = {spriteSize.x * size.x, spriteSize.y * size.y};
    }

    rect = {position, adjustedSize, type};
}

void CollisionComponent::update()
{
    if (rect.type == RectType::ACTIVE)
    {
        if (auto m = myEntity->getComponent<MovementComponent>())
        {
            // TILE COLLISION
            std::array<Chunk*, 9> nearbyChunks = myEntity->game->getScene()->getChunkLayer()->getNearbyChunks(myEntity->getPosition());
            // Chunk, position, z-value
            std::vector<std::pair<Chunk*, std::pair<sf::Vector2i, int>>> nearbyTilesWithColliders;

            for (int i = 0; i < nearbyChunks.size(); i++)
            {
                auto currChunkTWithC = &nearbyChunks[i]->tilesWithColliders;

                for (int j = 0; j < currChunkTWithC->size(); j++)
                {
                    Tile* currTile = (*currChunkTWithC)[j];

                    nearbyTilesWithColliders.push_back({nearbyChunks[i], {currTile->localPosition, currTile->z}});
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
                    Tile* tile = t.first->getTile(t.second.first.x, t.second.first.y, t.second.second);
                    sf::FloatRect tileRect = t.first->getTileRect(t.second.first, t.second.second);
                    WorldPosition tilePos(tileRect.position);
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
        
                for (auto i : collidingTiles)
                {
                    if (dynamicRectRectCollide(&rect, m->velocity, &i.first, contactPoint, contactNormal, contactTime))
                    {
                        m->velocity += sf::Vector2f(std::abs(m->velocity.x) * contactNormal.x, std::abs(m->velocity.y) * contactNormal.y) * (1.f - contactTime);
                        m->velocity += {contactNormal.x * .001f, contactNormal.y * .001f};
                    }
                }
            }

            // ENTITY COLLISION
            std::vector<Entity*> entities = myEntity->game->getScene()->getEntityLayer()->getEntitiesInChunkArea(rect.position.getPos(), 1);
        
            // <rect, contact time>
            std::vector<std::pair<CollisionRect*, float>> z;

            for (auto i : entities)
            {
                if (i->getID() == myEntity->getID()) continue;
        
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
}

void CollisionComponent::resolveCollision(CollisionRect* other, char direction, float pushFraction)
{
    pushFraction = std::min(1.f, pushFraction);

    float diff;

    if (direction == 'l')
    {
        diff = other->right() - rect.left();

        rect.move({diff * (1.f - pushFraction), 0});
        other->move({diff * pushFraction, 0});
    }
    else if (direction == 'r')
    {
        diff = other->left() - rect.right();

        rect.move({diff * (1.f - pushFraction), 0});
        other->move({diff * pushFraction, 0});
    }
    else if (direction == 't')
    {
        diff = other->bottom() - rect.top();

        rect.move({0, diff * (1.f - pushFraction)});
        other->move({0, diff * pushFraction});
    }
    else if (direction == 'b')
    {
        diff = other->top() - rect.bottom();

        rect.move({0, diff * (1.f - pushFraction)});
        other->move({0, diff * pushFraction});
    }
    else
    {
        std::cout << "no valid direction picked for resolveCollision() in CollisionComponent.\n";
        assert(false);
    }
}