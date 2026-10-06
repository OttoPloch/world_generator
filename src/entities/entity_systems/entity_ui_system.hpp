#pragma once

#include <vector>

class Game;
class Scene;
class EntityLayer;
class Entity;
class UIManagementSystem;

class EntityUISystem
{
public:
    EntityUISystem();

    EntityUISystem(Game* p_game, Scene* p_scene, UIManagementSystem* p_uiManagementSystem);

    void tick();

    void refactorEntityCache();
private:
    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;
    UIManagementSystem* uiManagementSystem;

    std::vector<Entity*> validEntities;
};