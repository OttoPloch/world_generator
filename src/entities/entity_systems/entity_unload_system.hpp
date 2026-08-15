#pragma once

class Game;
class Scene;
class EntityLayer;

class EntityUnloadSystem
{
public:
    EntityUnloadSystem();

    EntityUnloadSystem(Game* game, Scene* scene);

    void tick();
private:
    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;
};