#pragma once

class Game;
class Scene;
class EntityLayer;

class RenderSystem
{
public:
    RenderSystem();

    RenderSystem(Game* game, Scene* scene);

    void update(float dt);

    void draw();
private:
    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;
};