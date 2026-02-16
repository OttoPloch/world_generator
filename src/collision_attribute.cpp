#include "collision_attribute.hpp"
#include "collision_rect.hpp"
#include "motion_attribute.hpp"
#include "states.hpp"
#include "rect_types.hpp"
#include <cmath>
#include <cstdlib>

CollisionAttribute::CollisionAttribute() : Attribute("collision") {}

CollisionAttribute::CollisionAttribute(Entity* myEntity, GamePosition position, sf::Vector2f offset, sf::Vector2f size, std::string colliderName, int rectType, std::vector<std::string> blacklist) : Attribute("collision")
{
    this->myEntity = myEntity;

    states = myEntity->getStates();

    rect.init(position, offset, size, colliderName, rectType, blacklist);
}

void CollisionAttribute::tick(char axis, std::vector<std::unique_ptr<Entity>>* entities, std::vector<std::vector<Tile>*> surroundingTiles)
{
    rect.updatePosition();

    if (rect.getType() == ACTIVE || rect.getType() == MOVABLE)
    {
        std::vector<Entity*> entityCollisions;
        if (entities)entityCollisions = getEntityCollisions(entities);

        std::vector<Tile*> tileCollisions;
        if (surroundingTiles.size() > 0) tileCollisions = getTileCollisions(&surroundingTiles);

        int iterations = 0;
        while (entityCollisions.size() > 0 && iterations < 8)
        {
            iterations++;

            float smallestDiff = INFINITY;
            int closestEntityIndex = -1;

            for (int i = 0; i < entityCollisions.size(); i++)
            {
                Entity* entity = entityCollisions[i];

                CollisionRect* other = &entity->getCollision()->rect;

                sf::Vector2f relativeMovement = myEntity->getLastTickMovement() - entity->getLastTickMovement();

                float diff;

                if (axis == 'x')
                {
                    if (relativeMovement.x > 0.f)
                    {
                        diff = rect.right() - other->left();
                    }
                    else if (relativeMovement.x < 0.f)
                    {
                        diff = other->right() - rect.left();
                    }
                    else
                    {
                        if (rect.center().x < other->center().x) diff = rect.right() - other->left();
                        else diff = other->right() - rect.left();
                    }
                }
                else if (axis == 'y')
                {
                    if (relativeMovement.y > 0.f)
                    {
                        diff = rect.bottom() - other->top();
                    }
                    else if (relativeMovement.y < 0.f)
                    {
                        diff = other->bottom() - rect.top();
                    }
                    else
                    {
                        if (rect.center().y < other->center().y) diff = rect.bottom() - other->top();
                        else diff = other->bottom() - rect.top();
                    }
                }

                if (diff > 0 && diff < smallestDiff)
                {
                    smallestDiff = diff;

                    closestEntityIndex = i;
                }
            }

            if (closestEntityIndex == -1) break;

            Entity* entity = entityCollisions[closestEntityIndex];

            entityCollisions.erase(entityCollisions.begin() + closestEntityIndex);

            CollisionRect* other = &entity->getCollision()->rect;            
            MotionAttribute* myMotion = myEntity->getMotion();
            MotionAttribute* otherMotion = entity->getMotion();
            sf::Vector2f relativeMovement = myEntity->getLastTickMovement() - entity->getLastTickMovement();

            bool advancedCollision = (myMotion && otherMotion);
            bool pushingObject = false;

            if (rect.getType() == ACTIVE)
            {
                if (other->getType() == ACTIVE)
                {   
                    if (advancedCollision) resolveCollision(axis, other, relativeMovement, myMotion->getMass(), otherMotion->getMass());
                    else resolveCollision(axis, other, relativeMovement, 0.3f, 0.7f);
                }
                else if (other->getType() == STATIC)
                {
                    resolveCollision(axis, other, relativeMovement, myMotion->getMass(), INFINITY);
                }
                else if (other->getType() == MOVABLE)
                {
                    pushingObject = true;

                    if (advancedCollision) resolveCollision(axis, other, relativeMovement, myMotion->getMass(), otherMotion->getMass());
                    else resolveCollision(axis, other, relativeMovement, .5f, .5f);
                }
            }
            else if (rect.getType() == MOVABLE)
            {
                if (other->getType() == STATIC) resolveCollision(axis, other, relativeMovement, myMotion->getMass(), INFINITY);
                else if (other->getType() == MOVABLE)
                {
                    if (advancedCollision) resolveCollision(axis, other, relativeMovement, myMotion->getMass(), otherMotion->getMass());
                    else resolveCollision(axis, other, relativeMovement, .5f, .5f);
                }
            }

            states->set("collision", COLL_ANY);

            if (axis == 'x')
            {
                if (relativeMovement.x > 0.f)
                {
                    states->set("collision", COLL_RIGHT);

                    if (pushingObject) states->set("animation", ANIM_PUSHINGRIGHT);
                }
                else if (relativeMovement.x < 0.f)
                {
                    states->set("collision", COLL_LEFT);

                    if (pushingObject) states->set("animation", ANIM_PUSHINGLEFT);
                }
            }
            else if (axis == 'y')
            {
                if (relativeMovement.y > 0.f)
                {
                    states->set("collision", COLL_BOTTOM);

                    if (pushingObject) states->set("animation", ANIM_PUSHINGDOWN);
                }
                else if (relativeMovement.y < 0.f)
                {
                    states->set("collision", COLL_TOP);

                    if (pushingObject) states->set("animation", ANIM_PUSHINGUP);
                }
            }

            entityCollisions = getEntityCollisions(entities);
        }

        // while (tileCollisions.size() > 0)
        // {
        //     for (auto tile : tileCollisions)
        //     {
        //         if (!rect.searchBlacklist(tile->colliderName))
        //         {
        //             resolveCollision(axis, tile->getCollRect());

        //             states->set("collision", COLL_ANY);

        //             if (axis == 'x')
        //             {
        //                 if (myEntity->getLastTickMovement().x > 0.f)
        //                 {
        //                     states->set("collision", COLL_RIGHT);
        //                 }
        //                 else if (myEntity->getLastTickMovement().x < 0.f)
        //                 {
        //                     states->set("collision", COLL_LEFT);
        //                 }
        //             }
        //             else if (axis == 'y')
        //             {
        //                 if (myEntity->getLastTickMovement().y > 0.f)
        //                 {
        //                     states->set("collision", COLL_BOTTOM);
        //                 }
        //                 else if (myEntity->getLastTickMovement().y < 0.f)
        //                 {
        //                     states->set("collision", COLL_TOP);
        //                 }
        //             }
        //         }
        //     }
        // }
    }

    rect.lastPosition = rect.center();
}

std::vector<Entity*> CollisionAttribute::getEntityCollisions(std::vector<std::unique_ptr<Entity>>* entities)
{
    std::vector<Entity*> collisions;

    for (int i = 0; i < entities->size(); i++)
    {
        Entity* entity = (*entities)[i].get();

        if (entity->getID() != myEntity->getID())
        {
            if (entity->getCollision())
            {
                CollisionRect* other = &entity->getCollision()->rect;

                if (collidesWith(*other))
                {
                    if (!rect.searchBlacklist(other->getColliderName()))
                    {
                        if (!other->searchBlacklist(rect.getColliderName())) collisions.push_back(entity);
                    }
                }
            }
        }
    }

    return collisions;
}

std::vector<Tile*> CollisionAttribute::getTileCollisions(std::vector<std::vector<Tile>*>* surroundingTiles)
{
    return {};
}

bool CollisionAttribute::collidesWith(CollisionRect other)
{
    return (rect.left() < other.right() && rect.right() > other.left() && rect.top() < other.bottom() && rect.bottom() > other.top());
}

bool CollisionAttribute::collidesWith(sf::FloatRect other)
{
    return (rect.left() < other.position.x + other.size.x && rect.right() > other.position.x && rect.top() < other.position.y + other.size.y && rect.bottom() > other.position.y);
}

void CollisionAttribute::resolveCollision(char axis, CollisionRect* other, sf::Vector2f relMove, float myMass, float otherMass)
{
    float invA = 1 / myMass;
    float invB = 1 / otherMass;
    float sum = invA + invB;

    MotionAttribute* myMotion = myEntity->getMotion();

    if (axis == 'x')
    {
        if (relMove.x > 0.f)
        {
            float diff = rect.right() - other->left();
            if (diff <= 0.f) return;

            rect.setRight(rect.right() - diff * (invA / sum));
            other->setLeft(other->left() + diff * (invB / sum));

            if (myMotion) myMotion->setVelocity('x', 0.f);
        }
        else if (relMove.x < 0.f)
        {
            float diff = other->right() - rect.left();
            if (diff <= 0.f) return;

            rect.setLeft(rect.left() + diff * (invA / sum));
            other->setRight(other->right() - diff * (invB / sum));

            if (myMotion) myMotion->setVelocity('x', 0.f);
        }
    }
    else if (axis == 'y')
    {
        if (relMove.y > 0.f)
        {
            float diff = rect.bottom() - other->top();
            if (diff <= 0.f) return;

            rect.setBottom(rect.bottom() - diff * (invA / sum));
            other->setTop(other->top() + diff * (invB / sum));

            if (myMotion) myMotion->setVelocity('y', 0.f);
        }
        else if (relMove.y < 0.f)
        {
            float diff = other->bottom() - rect.top();
            if (diff <= 0.f) return;

            rect.setTop(rect.top() + diff * (invA / sum));
            other->setBottom(other->bottom() - diff * (invB / sum));

            if (myMotion) myMotion->setVelocity('y', 0.f);
        }
    }
}

void CollisionAttribute::resolveCollision(char axis, sf::FloatRect other)
{
    if (axis == 'x')
    {
        if (myEntity->getLastTickMovement().x > 0.f)
        {
            rect.setRight(other.position.x);
        }
        else if (myEntity->getLastTickMovement().x < 0.f)
        {
            rect.setLeft(other.position.x + other.size.x);
        }
    }
    else if (axis == 'y')
    {
        if (myEntity->getLastTickMovement().y > 0.f)
        {
            rect.setBottom(other.position.y);
        }
        else if (myEntity->getLastTickMovement().y < 0.f)
        {
            rect.setTop(other.position.y + other.size.y);
        }
    }
}

void CollisionAttribute::setRect(sf::FloatRect newRect)
{
    rect.setOffset(newRect.position);
    
    rect.setSize(newRect.size);
}

CollisionRect* CollisionAttribute::getRect() { return &rect; }

Entity* CollisionAttribute::getEntity() { return myEntity; }