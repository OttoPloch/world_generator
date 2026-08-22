#pragma once

class Game;
class Scene;
class EntityLayer;

class ItemSystem
{
public:
    ItemSystem();

    ItemSystem(Game* game, Scene* scene);

    void tick();
private:
    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;
};