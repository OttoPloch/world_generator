#pragma once

class Game;
class Scene;
class EntityLayer;

class AnimationSystem
{
public:
    AnimationSystem();

    AnimationSystem(Game* game, Scene* scene);

    void update(float dt);
private:
    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;
};