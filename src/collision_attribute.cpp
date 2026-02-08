#include "collision_attribute.hpp"
#include "states.hpp"
#include "rect_types.hpp"

CollisionAttribute::CollisionAttribute() : Attribute("collision") {}

CollisionAttribute::CollisionAttribute(Entity* myEntity, GamePosition position, sf::Vector2f offset, sf::Vector2f size, std::string colliderName, int rectType, std::vector<std::string> blacklist) : Attribute("collision")
{
    this->myEntity = myEntity;

    states = myEntity->getStates();

    rect.init(position, offset, size, colliderName, rectType, blacklist);
}

void CollisionAttribute::tick(std::vector<std::unique_ptr<Entity>>* entities, std::vector<Tile>* tiles)
{
    rect.updatePosition();

    if (rect.getType() == ACTIVE || rect.getType() == MOVABLE)
    {
        if (entities != nullptr)
        {
            for (int i = 0; i < entities->size(); i++)
            {
                Entity* entity = (*entities)[i].get();

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

                                    MotionAttribute* myMotion = myEntity->getMotion();
                                    MotionAttribute* otherMotion = entity->getMotion();

                                    bool advancedCollision = (myMotion && otherMotion);

                                    if (rect.getType() == ACTIVE)
                                    {
                                        if (other->getType() == ACTIVE)
                                        {   
                                            if (advancedCollision)
                                            {
                                                float pushFraction = (myMotion->getMass() / (myMotion->getMass() + otherMotion->getMass()));

                                                resolveCollision(other, pushFraction);
                                            }
                                            else
                                            {
                                                resolveCollision(other, (0.3f));
                                            }
                                        }
                                        else if (other->getType() == STATIC)
                                        {
                                            resolveCollision(other, 0.f);
                                        }
                                        else if (other->getType() == MOVABLE)
                                        {
                                            pushingObject = true;

                                            if (advancedCollision)
                                            {
                                                float pushFraction = (myMotion->getMass() / (myMotion->getMass() + otherMotion->getMass()));

                                                resolveCollision(other, pushFraction);
                                            }
                                            else
                                            {
                                                resolveCollision(other, (0.5f));
                                            }
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
                                            if (advancedCollision)
                                            {
                                                float pushFraction = (myMotion->getMass() / (myMotion->getMass() + otherMotion->getMass()));

                                                resolveCollision(other, pushFraction);
                                            }
                                            else
                                            {
                                                resolveCollision(other, (0.5f));
                                            }
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

        if (tiles != nullptr)
        {
            for (int i = 0; i < tiles->size(); i++)
            {
                Tile* tile = &(*tiles)[i];

                if (tile->hasCollider())
                {
                    sf::FloatRect other = tile->getRect();

                    if (collidesWith(other))
                    {
                        if (!rect.searchBlacklist(tile->getColliderName()))
                        {
                            resolveCollision(other);

                            states->set("collision", COLL_ANY);

                            float leftDiff = abs(rect.right() - other.position.x);
                            float rightDiff = abs(rect.left() - other.position.x + other.size.x);
                            float topDiff = abs(rect.bottom() - other.position.y);
                            float bottomDiff = abs(rect.top() - other.position.y + other.size.y);

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
        }
    }

    rect.lastPosition = rect.center();
}

bool CollisionAttribute::collidesWith(CollisionRect* other)
{
    return (rect.left() < other->right() && rect.right() > other->left() && rect.top() < other->bottom() && rect.bottom() > other->top());
}

bool CollisionAttribute::collidesWith(sf::FloatRect other)
{
    return (rect.left() < other.position.x + other.size.x && rect.right() > other.position.x && rect.top() < other.position.y + other.size.y && rect.bottom() > other.position.y);
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

void CollisionAttribute::resolveCollision(sf::FloatRect other)
{
    float leftDiff = abs(rect.right() - other.position.x);
    float rightDiff = abs(rect.left() - (other.position.x + other.size.x));
    float topDiff = abs(rect.bottom() - other.position.y);
    float bottomDiff = abs(rect.top() - (other.position.y + other.size.y));

    if (leftDiff < rightDiff && leftDiff < topDiff && leftDiff < bottomDiff)
    {
        float diff = other.position.x - rect.right();

        rect.setRight(rect.right() + diff);
    }
    if (rightDiff < leftDiff && rightDiff < topDiff && rightDiff < bottomDiff)
    {
        float diff = other.position.x + other.size.x - rect.left();
        
        rect.setLeft(rect.left() + diff);
    }
    if (topDiff < leftDiff && topDiff < rightDiff && topDiff < bottomDiff)
    {
        float diff = other.position.y - rect.bottom();

        rect.setBottom(rect.bottom() + diff);
    }
    if (bottomDiff < leftDiff && bottomDiff < rightDiff && bottomDiff < topDiff)
    {
        float diff = other.position.y + other.size.y - rect.top();

        rect.setTop(rect.top() + diff);
    }
}

void CollisionAttribute::setRect(sf::FloatRect newRect)
{
    rect.setOffset(newRect.position);
    
    rect.setSize(newRect.size);
}

CollisionRect* CollisionAttribute::getRect() { return &rect; }

Entity* CollisionAttribute::getEntity() { return myEntity; }