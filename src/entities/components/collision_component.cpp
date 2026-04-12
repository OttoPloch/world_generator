#include "collision_component.hpp"
#include "../entity.hpp"
#include "../../core/game.hpp"
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
            std::vector<Entity*> entities = myEntity->game->getScene()->getEntityLayer()->getEntitiesInChunkArea(rect.position.getPos(), 1);
        
            std::vector<std::pair<CollisionRect*, float>> z;
            sf::Vector2f contactPoint;
            sf::Vector2f contactNormal;
            float contactTime;
    
            for (auto i : entities)
            {
                if (i->getID() == myEntity->getID()) continue;
        
                if (auto c = i->getComponent<CollisionComponent>())
                {
                    CollisionRect* other = &c->rect;
    
                    if (dynamicRectRectCollide(&rect, m->velocity, other, contactPoint, contactNormal, contactTime))
                    {
                        z.emplace_back(std::pair(&c->rect, contactTime));
                    }
                }
            }
    
            std::sort(z.begin(), z.end(), [](const std::pair<CollisionRect*, float>& a, const std::pair<CollisionRect*, float>& b)
            {
                return a.second < b.second;
            });
    
            for (auto j : z)
            {
                if (dynamicRectRectCollide(&rect, m->velocity, j.first, contactPoint, contactNormal, contactTime))
                {
                    m->velocity += sf::Vector2f(std::abs(m->velocity.x) * contactNormal.x, std::abs(m->velocity.y) * contactNormal.y) * (1.f - contactTime);
                    m->velocity += {contactNormal.x * .001f, contactNormal.y * .001f};
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