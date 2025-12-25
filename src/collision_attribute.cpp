#include "collision_attribute.hpp"
#include "states.hpp"
#include "rect_types.hpp"

CollisionAttribute::CollisionAttribute(Entity* myEntity, EntityStates* states, GamePosition position, sf::Vector2f offset, sf::Vector2f size, std::vector<Entity>* entities, std::string colliderName, int rectType, std::vector<std::string> blacklist) : Attribute("collision")
{
    this->myEntity = myEntity;

    this->states = states;

    rect.init(position, offset, size, colliderName, rectType, blacklist);

    this->entities = entities;
}

void CollisionAttribute::tick()
{
    rect.updatePosition();

    if (rect.getType() == ACTIVE || rect.getType() == MOVABLE)
    {
        for (int i = 0; i < entities->size(); i++)
        {
            Entity* entity = &(*entities)[i];

            if (entity->getID() != myEntity->getID())
            {
                if (entity->getCollision() != nullptr)
                {
                    CollisionRect* other = &entity->getCollision()->rect;
                    
                    if (collidesWith(other))
                    {
                        if (!rect.searchBlacklist(other->getColliderName()))
                        {
                            if (!other->searchBlacklist(rect.getColliderName()))
                            {
                                bool pushingObject = false;

                                if (rect.getType() == ACTIVE)
                                {
                                    if (other->getType() == ACTIVE)
                                    {   
                                        resolveCollision(other, 0.3f);
                                    }
                                    else if (other->getType() == STATIC)
                                    {
                                        resolveCollision(other, 0.f);
                                    }
                                    else if (other->getType() == MOVABLE)
                                    {
                                        pushingObject = true;

                                        resolveCollision(other, .5f);
                                    }
                                }
                                else if (rect.getType() == MOVABLE)
                                {
                                    if (other->getType() == ACTIVE)
                                    {
                                        // skip
                                    }
                                    else if (other->getType() == STATIC)
                                    {
                                        resolveCollision(other, 0.f);
                                    }
                                    else if (other->getType() == MOVABLE)
                                    {
                                        resolveCollision(other, 0.5f);
                                    }
                                }

                                states->set("collision", COLL_ANY);

                                float leftDiff = abs(rect.right() - other->left());
                                float rightDiff = abs(rect.left() - other->right());
                                float topDiff = abs(rect.bottom() - other->top());
                                float bottomDiff = abs(rect.top() - other->bottom());

                                if (leftDiff <= rightDiff && leftDiff <= topDiff && leftDiff <= bottomDiff)
                                {
                                    states->set("collision", COLL_RIGHT);

                                    if (pushingObject) states->set("animation", ANIM_PUSHINGRIGHT);
                                }
                                if (rightDiff <= leftDiff && rightDiff <= topDiff && rightDiff <= bottomDiff)
                                {
                                    states->set("collision", COLL_LEFT);
                                    
                                    if (pushingObject) states->set("animation", ANIM_PUSHINGLEFT);
                                }
                                if (topDiff <= leftDiff && topDiff <= rightDiff && topDiff <= bottomDiff)
                                {
                                    states->set("collision", COLL_BOTTOM);

                                    if (pushingObject) states->set("animation", ANIM_PUSHINGDOWN);
                                }
                                if (bottomDiff <= leftDiff && bottomDiff <= rightDiff && bottomDiff <= topDiff)
                                {
                                    states->set("collision", COLL_TOP);
                                    
                                    if (pushingObject) states->set("animation", ANIM_PUSHINGUP);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    rect.lastPosition = rect.center();
}

bool CollisionAttribute::collidesWith(CollisionRect* other)
{
    if (rect.left() < other->right() && rect.right() > other->left() && rect.top() < other->bottom() && rect.bottom() > other->top())
    {
        return true;
    }

    return false;
}

void CollisionAttribute::resolveCollision(CollisionRect* other, float pushFraction)
{
    float leftDiff = abs(rect.right() - other->left());
    float rightDiff = abs(rect.left() - other->right());
    float topDiff = abs(rect.bottom() - other->top());
    float bottomDiff = abs(rect.top() - other->bottom());

    if (leftDiff < rightDiff && leftDiff < topDiff && leftDiff < bottomDiff)
    {
        float diff = other->left() - rect.right();

        float thisMove = diff * (1.f - pushFraction);
        float otherMove = diff * pushFraction;

        rect.setRight(rect.right() + thisMove);
        other->setLeft(other->left() - otherMove);
    }
    if (rightDiff < leftDiff && rightDiff < topDiff && rightDiff < bottomDiff)
    {
        float diff = other->right() - rect.left();

        float thisMove = diff * (1.f - pushFraction);
        float otherMove = diff * pushFraction;

        rect.setLeft(rect.left() + thisMove);
        other->setRight(other->right() - otherMove);
    }
    if (topDiff < leftDiff && topDiff < rightDiff && topDiff < bottomDiff)
    {
        float diff = other->top() - rect.bottom();

        float thisMove = diff * (1.f - pushFraction);
        float otherMove = diff * pushFraction;

        rect.setBottom(rect.bottom() + thisMove);
        other->setTop(other->top() - otherMove);
    }
    if (bottomDiff < leftDiff && bottomDiff < rightDiff && bottomDiff < topDiff)
    {
        float diff = other->bottom() - rect.top();

        float thisMove = diff * (1.f - pushFraction);
        float otherMove = diff * pushFraction;

        rect.setTop(rect.top() + thisMove);
        other->setBottom(other->bottom() - otherMove);
    }
}

void CollisionAttribute::setRect(sf::FloatRect newRect)
{
    rect.setOffset(newRect.position);
    
    rect.setSize(newRect.size);
}

CollisionRect* CollisionAttribute::getRect() { return &rect; }