#include "collision_attribute.hpp"

CollisionAttribute::CollisionAttribute(int myEntityID, GamePosition position, sf::Vector2f size, std::vector<Entity>* entities, bool active) : Attribute("collision")
{
    this->myEntityID = myEntityID;

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

        if (entity->getID() != myEntityID)
        {
            if (entity->getCollision() != nullptr)
            {
                CollisionRect* other = &entity->getCollision()->rect;

                if (rect.left() < other->right() && rect.right() > other->left() && rect.top() < other->bottom() && rect.bottom() > other->top())
                {   
                    rect.setLeft(other->right());

                    std::cout << myEntityID << " TOUCHING " << entity->getID() << '\n';
                }
            }
        }
    }
}