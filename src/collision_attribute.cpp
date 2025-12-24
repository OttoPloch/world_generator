#include "collision_attribute.hpp"
#include "states.hpp"

CollisionAttribute::CollisionAttribute(Entity* myEntity, EntityStates* states, GamePosition position, sf::Vector2f offset, sf::Vector2f size, std::vector<Entity>* entities, bool active, std::string colliderName) : Attribute("collision")
{
    this->myEntity = myEntity;

    this->states = states;

    this->colliderName = colliderName;

    rect.init(position, offset, size);

    this->entities = entities;

    this->active = active;
}

void CollisionAttribute::tick()
{
    rect.updatePosition();

    for (int i = 0; i < entities->size(); i++)
    {
        Entity* entity = &(*entities)[i];

        if (entity->getID() != myEntity->getID())
        {
            if (entity->getCollision() != nullptr)
            {
                CollisionRect* other = &entity->getCollision()->rect;
                
                float leftDiff = abs(rect.right() - other->left());
                float rightDiff = abs(rect.left() - other->right());
                float topDiff = abs(rect.bottom() - other->top());
                float bottomDiff = abs(rect.top() - other->bottom());

                if (rect.left() < other->right() && rect.right() > other->left() && rect.top() < other->bottom() && rect.bottom() > other->top())
                {   
                    if (leftDiff < rightDiff && leftDiff < topDiff && leftDiff < bottomDiff)
                    {
                        rect.setRight(other->left());
                    }
                    if (rightDiff < leftDiff && rightDiff < topDiff && rightDiff < bottomDiff)
                    {
                        rect.setLeft(other->right());
                    }
                    if (topDiff < leftDiff && topDiff < rightDiff && topDiff < bottomDiff)
                    {
                        rect.setBottom(other->top());
                    }
                    if (bottomDiff < leftDiff && bottomDiff < rightDiff && bottomDiff < topDiff)
                    {
                        rect.setTop(other->bottom());
                    }
                }
                
                if (rect.left() <= other->right() && rect.right() >= other->left() && rect.top() <= other->bottom() && rect.bottom() >= other->top())
                {
                    states->set("collision", COLL_ANY);

                    if (leftDiff <= rightDiff && leftDiff <= topDiff && leftDiff <= bottomDiff)
                    {
                        states->set("collision", COLL_RIGHT);
                    }
                    if (rightDiff <= leftDiff && rightDiff <= topDiff && rightDiff <= bottomDiff)
                    {
                        states->set("collision", COLL_LEFT);
                    }
                    if (topDiff <= leftDiff && topDiff <= rightDiff && topDiff <= bottomDiff)
                    {
                        states->set("collision", COLL_BOTTOM);
                    }
                    if (bottomDiff <= leftDiff && bottomDiff <= rightDiff && bottomDiff <= topDiff)
                    {
                        states->set("collision", COLL_TOP);
                    }
                }
            }
        }
    }

    rect.lastPosition = rect.center();
}

void CollisionAttribute::setRect(sf::FloatRect newRect)
{
    rect.setOffset(newRect.position);
    
    rect.setSize(newRect.size);
}

CollisionRect* CollisionAttribute::getRect() { return &rect; }

std::string CollisionAttribute::getColliderName() { return colliderName; }