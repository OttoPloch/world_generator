#pragma once

#include <vector>

class Game;
class Scene;
class EntityLayer;
class Entity;

class MobSystem
{
public:
    MobSystem();

    MobSystem(Game* p_game, Scene* p_scene);

    void tick();

    void refactorEntityCache();
private:
    Game* m_game;
    Scene* m_scene;
    EntityLayer* m_entityLayer;

    std::vector<Entity*> m_validEntities;
};