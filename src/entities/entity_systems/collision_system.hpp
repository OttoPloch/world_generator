#pragma once

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

    void update(float dt);

private:
    void findAndResolveCollisions(Entity* e, MovementComponent* m, CollisionComponent* c);

    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;
};