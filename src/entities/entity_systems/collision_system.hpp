#pragma once

#include "../collision_rect.hpp"

class Game;
class Scene;
class EntityLayer;
class Entity;
struct MovementComponent;
struct CollisionComponent;
struct PositionComponent;

class CollisionSystem
{
public:
    CollisionSystem();

    CollisionSystem(Game* game, Scene* scene);

    void tick();
private:
    void findAndResolveCollisions(Entity* e);

    void tileCollision(CollisionRect& rect, MovementComponent* m, PositionComponent* p);

    void entityCollision(Entity* e, CollisionRect& rect, MovementComponent* m);

    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;
};