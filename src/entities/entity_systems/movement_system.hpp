#pragma once

class Game;
class Scene;
class EntityLayer;

class MovementSystem
{
public:
    MovementSystem();

    MovementSystem(Game* game, Scene* scene);

    void tick();

    void update(float dt);
private:
    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;
};