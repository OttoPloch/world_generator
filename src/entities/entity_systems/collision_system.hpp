#pragma once

#include "../collision_rect.hpp"

class Game;
class Scene;
class EntityLayer;
class Entity;
struct MovementComponent;
struct CollisionComponent;

class CollisionSystem
{
public:
    CollisionSystem();

    CollisionSystem(Game* game, Scene* scene);

    void tick();

    void refactorEntityCache();
private:
    void findAndResolveCollisions(Entity* entity, MovementComponent* entityMovementComponent, CollisionComponent* entityCollisionComponent);

    void tileCollision(Entity* entity, CollisionRect& rect, MovementComponent* entityMovementComponent);

    void entityCollision(Entity* entity, CollisionRect& rect, MovementComponent* entityMovementComponent);

    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;

    std::vector<Entity*> validEntities;
};