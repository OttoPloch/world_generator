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

    void draw(bool debug);
private:
    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;
};