#pragma once

class Game;
class Scene;
class EntityLayer;

class PositionSystem
{
public:
    PositionSystem();

    PositionSystem(Game* game, Scene* scene);

    void tick();
private:
    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;
};