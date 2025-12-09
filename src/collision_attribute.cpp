#include "collision_attribute.hpp"

CollisionAttribute::CollisionAttribute(Entity* myEntity, GamePosition position, sf::Vector2f size, std::vector<Entity>* entities, bool active) : Attribute("collision")
{
    this->myEntity = myEntity;

    this->position = position;

    this->size = size;

    rect.init(position, size);

    this->entities = entities;

    this->active = active;
}

void CollisionAttribute::tick()
{
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
                    float leftDiff = abs(rect.center().x - other->left());
                    float rightDiff = abs(rect.center().x - other->right());
                    float topDiff = abs(rect.center().y - other->top());
                    float bottomDiff = abs(rect.center().y - other->bottom());

                    int choice = 0;

                    if (leftDiff < rightDiff && leftDiff > topDiff && leftDiff > bottomDiff)
                    {
                        rect.setRight(other->left());

                        choice = 1;
                    }
                    else if (rightDiff < leftDiff && rightDiff > topDiff && rightDiff > bottomDiff)
                    {
                        rect.setLeft(other->right());

                        choice = 2;
                    }
                    else if (topDiff > leftDiff && topDiff > rightDiff && topDiff < bottomDiff)
                    {
                        rect.setBottom(other->top());

                        choice = 3;
                    }
                    else if (bottomDiff > leftDiff && bottomDiff > rightDiff && bottomDiff < topDiff)
                    {
                        rect.setTop(other->bottom());

                        choice = 4;
                    }

                    std::cout << "/////////////////////////////////////////\n";
                    std::cout << myEntity->getID() << " TOUCHING " << entity->getID() << '\n';
                    if (choice == 1) std::cout << "left > "; std::cout << leftDiff << '\n';
                    if (choice == 2) std::cout << "right > "; std::cout << rightDiff << '\n';
                    if (choice == 3) std::cout << "top > "; std::cout << topDiff << '\n';
                    if (choice == 4) std::cout << "bottom > "; std::cout << bottomDiff << '\n';
                    std::cout << "/////////////////////////////////////////\n";
                }
            }
        }
    }
}