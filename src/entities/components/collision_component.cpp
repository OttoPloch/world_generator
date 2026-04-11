#include "collision_component.hpp"
#include "../entity.hpp"
#include "../../core/game.hpp"
#include "entity_component.hpp"

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
        auto entities = myEntity->game->getScene()->getEntityLayer()->getEntitiesInChunkArea(rect.position.getPos(), 1);
    
        for (auto i : entities)
        {
            if (i->getID() == myEntity->getID()) continue;
    
            if (auto c = i->getComponent<CollisionComponent>())
            {
                CollisionRect* other = &c->rect;

                if (rectRectCollide({rect.position.getPos(), rect.size}, {other->position.getPos(), other->size}, true))
                {
                    float leftDiff = std::fabs(rect.left() - other->right());
                    float rightDiff = std::fabs(other->left() - rect.right());
                    float topDiff = std::fabs(rect.top() - other->bottom());
                    float bottomDiff = std::fabs(other->top() - rect.bottom());

                    char direction;
                    float pushFraction;

                    if (other->type == RectType::ACTIVE) pushFraction = .5f;
                    else if (other->type == RectType::PASSIVE) pushFraction = 1;
                    else if (other->type == RectType::STATIC) pushFraction = 0;

                    if (leftDiff < rightDiff && leftDiff < topDiff && leftDiff < bottomDiff)
                    {
                        direction = 'l';
                    }
                    else if (rightDiff < leftDiff && rightDiff < topDiff && rightDiff < bottomDiff)
                    {
                        direction = 'r';
                    }
                    else if (topDiff < leftDiff && topDiff < rightDiff && topDiff < bottomDiff)
                    {
                        direction = 't';
                    }
                    else if (bottomDiff < leftDiff && bottomDiff < rightDiff && bottomDiff < topDiff)
                    {
                        direction = 'b';
                    }
                    else
                    {
                        std::cout << "diffs: " << leftDiff << ", " << rightDiff << ", " << topDiff << ", " << bottomDiff << '\n';
                        std::cout << "figure this out\n";
                        assert(false);
                    }

                    std::cout << direction << ", " << pushFraction << '\n';

                    resolveCollision(other, direction, pushFraction);
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