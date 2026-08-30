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
private:
    void findAndResolveCollisions(Entity* e);

    void tileCollision(Entity* e, CollisionRect& rect, MovementComponent* m);

    void entityCollision(Entity* e, CollisionRect& rect, MovementComponent* m);

    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;
};