// #include "collision_attribute.hpp"
// #include "collision_rect.hpp"
// #include "motion_attribute.hpp"
// #include "states.hpp"
// #include "rect_types.hpp"
// #include "utils.hpp"
// #include <cmath>
// #include <cstdlib>

// CollisionAttribute::CollisionAttribute() : Attribute("collision") {}

// CollisionAttribute::CollisionAttribute(Entity* myEntity, GamePosition position, sf::Vector2f offset, sf::Vector2f size, std::string colliderName, int rectType, std::vector<std::string> blacklist) : Attribute("collision")
// {
//     this->myEntity = myEntity;

//     states = myEntity->getStates();

//     rect.init(position, offset, size, colliderName, rectType, blacklist);
// }

// void CollisionAttribute::tick(std::vector<std::unique_ptr<Entity>>* entities, std::vector<std::vector<Tile>*> surroundingTiles)
// {
//     rect.updatePosition();

//     if (rect.getType() == ACTIVE || rect.getType() == MOVABLE)
//     {
//         std::vector<Entity*> entityCollisions;
//         if (entities)entityCollisions = getEntityCollisions(entities);

//         std::vector<Tile*> tileCollisions;
//         if (surroundingTiles.size() > 0) tileCollisions = getTileCollisions(&surroundingTiles);

//         MotionAttribute* myMotion = myEntity->getMotion();

//         if (entityCollisions.size() > 0) states->set("collision", COLL_ANY);

//         for (auto entity : entityCollisions)
//         {
//             CollisionRect* other = entity->getCollision()->getRect();
//             MotionAttribute* otherMotion = entity->getMotion();

//             bool advancedCollision = (myMotion && otherMotion);

//             float myMass;
//             float otherMass;

//             if (advancedCollision)
//             {
//                 myMass = myMotion->getMass();
//                 otherMass = otherMotion->getMass();
//             }
//             else
//             {
//                 myMass = 1.f;
//                 otherMass = 1.f;
//             }

//             bool pushingObject = other->getType() == MOVABLE;

//             if (rect.getType() == ACTIVE)
//             {
//                 if (other->getType() == ACTIVE || other->getType() == MOVABLE)
//                 {
//                     resolveCollision(other, pushingObject, myMass, otherMass);
//                 }
//                 else if (other->getType() == STATIC)
//                 {
//                     resolveCollision(other, pushingObject, myMass, INFINITY);
//                 }
//             }
//             else if (rect.getType() == MOVABLE)
//             {
//                 if (other->getType() == MOVABLE)
//                 {
//                     resolveCollision(other, pushingObject, myMass, otherMass);
//                 }
//                 else if (other->getType() == STATIC)
//                 {
//                     resolveCollision(other, pushingObject, myMass, INFINITY);
//                 }
//             }
//         }

//         // while (tileCollisions.size() > 0)
//         // {
//         //     for (auto tile : tileCollisions)
//         //     {
//         //         if (!rect.searchBlacklist(tile->colliderName))
//         //         {
//         //             resolveCollision(axis, tile->getCollRect());

//         //             states->set("collision", COLL_ANY);

//         //             if (axis == 'x')
//         //             {
//         //                 if (myEntity->getLastTickMovement().x > 0.f)
//         //                 {
//         //                     states->set("collision", COLL_RIGHT);
//         //                 }
//         //                 else if (myEntity->getLastTickMovement().x < 0.f)
//         //                 {
//         //                     states->set("collision", COLL_LEFT);
//         //                 }
//         //             }
//         //             else if (axis == 'y')
//         //             {
//         //                 if (myEntity->getLastTickMovement().y > 0.f)
//         //                 {
//         //                     states->set("collision", COLL_BOTTOM);
//         //                 }
//         //                 else if (myEntity->getLastTickMovement().y < 0.f)
//         //                 {
//         //                     states->set("collision", COLL_TOP);
//         //                 }
//         //             }
//         //         }
//         //     }
//         // }
//     }

//     rect.lastPosition = rect.center();
// }

// std::vector<Entity*> CollisionAttribute::getEntityCollisions(std::vector<std::unique_ptr<Entity>>* entities)
// {
//     std::vector<Entity*> collisions;

//     for (int i = 0; i < entities->size(); i++)
//     {
//         Entity* entity = (*entities)[i].get();

//         if (entity->getID() != myEntity->getID())
//         {
//             if (entity->getCollision())
//             {
//                 CollisionRect* other = &entity->getCollision()->rect;

//                 if (collidesWith(*other))
//                 {
//                     if (!rect.searchBlacklist(other->getColliderName()))
//                     {
//                         if (!other->searchBlacklist(rect.getColliderName())) collisions.push_back(entity);
//                     }
//                 }
//             }
//         }
//     }

//     return collisions;
// }

// std::vector<Tile*> CollisionAttribute::getTileCollisions(std::vector<std::vector<Tile>*>* surroundingTiles)
// {
//     return {};
// }

// bool CollisionAttribute::collidesWith(CollisionRect other)
// {
//     return (rect.left() < other.right() && rect.right() > other.left() && rect.top() < other.bottom() && rect.bottom() > other.top());
// }

// bool CollisionAttribute::collidesWith(sf::FloatRect other)
// {
//     return (rect.left() < other.position.x + other.size.x && rect.right() > other.position.x && rect.top() < other.position.y + other.size.y && rect.bottom() > other.position.y);
// }

// void CollisionAttribute::resolveCollision(CollisionRect* other, bool pushingObject, float myMass, float otherMass)
// {
//     float invA = 1 / myMass;
//     float invB = 1 / otherMass;
//     float sum = invA + invB;

//     float leftDiff = rect.right() - other->left();
//     float rightDiff = other->right() - rect.left();
//     float topDiff = rect.bottom() - other->top();
//     float bottomDiff = other->bottom() - rect.top();

//     float thisMove;
//     float otherMove;

//     int side;

//     if (leftDiff < rightDiff && leftDiff < topDiff && leftDiff < bottomDiff)
//     {
//         side = 0;
//     }
//     else if (rightDiff < leftDiff && rightDiff < topDiff && rightDiff < bottomDiff)
//     {
//         side = 1;
//     }
//     else if (topDiff < leftDiff && topDiff < rightDiff && topDiff < bottomDiff)
//     {
//         side = 2;
//     }
//     else if (bottomDiff < leftDiff && bottomDiff < rightDiff && bottomDiff < topDiff)
//     {
//         side = 3;
//     }
//     else
//     {
//         side = getRandInt(0, 3);
//     }

//     switch (side)
//     {
//         case 0:
//             thisMove = leftDiff * (invA / sum);
//             otherMove = leftDiff * (invB / sum);
//             rect.setRight(rect.right() - thisMove);
//             other->setLeft(other->left() + otherMove);
//             states->set("collision", COLL_RIGHT);
//             if (pushingObject) states->set("animation", ANIM_PUSHINGRIGHT);
//             break;
//         case 1:
//             thisMove = rightDiff * (invA / sum);
//             otherMove = rightDiff * (invB / sum);
//             rect.setLeft(rect.left() + thisMove);
//             other->setRight(other->right() - otherMove);
//             states->set("collision", COLL_LEFT);
//             if (pushingObject) states->set("animation", ANIM_PUSHINGLEFT);
//             break;
//         case 2:
//             thisMove = topDiff * (invA / sum);
//             otherMove = topDiff * (invB / sum);
//             rect.setBottom(rect.bottom() - thisMove);
//             other->setTop(other->top() + otherMove);
//             states->set("collision", COLL_BOTTOM);
//             if (pushingObject) states->set("animation", ANIM_PUSHINGDOWN);
//             break;
//         case 3:
//             thisMove = bottomDiff * (invA / sum);
//             otherMove = bottomDiff * (invB / sum);
//             rect.setTop(rect.top() + thisMove);
//             other->setBottom(other->bottom() - otherMove);
//             states->set("collision", COLL_TOP);
//             if (pushingObject) states->set("animation", ANIM_PUSHINGUP);
//             break;
//     }
// }

// void CollisionAttribute::resolveCollision(sf::FloatRect other)
// {
//     // if (axis == 'x')
//     // {
//     //     if (myEntity->getLastTickMovement().x > 0.f)
//     //     {
//     //         rect.setRight(other.position.x);
//     //     }
//     //     else if (myEntity->getLastTickMovement().x < 0.f)
//     //     {
//     //         rect.setLeft(other.position.x + other.size.x);
//     //     }
//     // }
//     // else if (axis == 'y')
//     // {
//     //     if (myEntity->getLastTickMovement().y > 0.f)
//     //     {
//     //         rect.setBottom(other.position.y);
//     //     }
//     //     else if (myEntity->getLastTickMovement().y < 0.f)
//     //     {
//     //         rect.setTop(other.position.y + other.size.y);
//     //     }
//     // }
// }

// void CollisionAttribute::setRect(sf::FloatRect newRect)
// {
//     rect.setOffset(newRect.position);
    
//     rect.setSize(newRect.size);
// }

// CollisionRect* CollisionAttribute::getRect() { return &rect; }

// Entity* CollisionAttribute::getEntity() { return myEntity; }