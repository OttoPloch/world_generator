#include "collision_attribute.hpp"

CollisionAttribute::CollisionAttribute(Entity* myEntity, GamePosition position, sf::Vector2f offset, sf::Vector2f size, std::vector<Entity>* entities, bool active, std::string name) : Attribute("collision")
{
    this->myEntity = myEntity;

    this->name = name;

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
                
                if (rect.left() < other->right() && rect.right() > other->left() && rect.top() < other->bottom() && rect.bottom() > other->top())
                {   
                    float leftDiff = abs(rect.right() - other->left());
                    float rightDiff = abs(rect.left() - other->right());
                    float topDiff = abs(rect.bottom() - other->top());
                    float bottomDiff = abs(rect.top() - other->bottom());

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
            }
        }
    }

    rect.lastPosition = rect.center();
}

CollisionRect CollisionAttribute::getRect() { return rect; }