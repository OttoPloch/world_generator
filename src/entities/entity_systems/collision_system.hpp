#pragma once

class Game;
class Scene;
class EntityLayer;

class CollisionSystem
{
public:
    CollisionSystem();

    CollisionSystem(Game* game, Scene* scene);

    void update(float dt);
private:
    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;
};