#pragma once

class Game;
class Scene;
class EntityLayer;

class ActionSystem
{
public:
    ActionSystem();

    ActionSystem(Game* game, Scene* scene);

    void update(float dt);
private:
    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;
};