#pragma once

// TEMP FOR DEBUG
#include <SFML/Graphics.hpp>

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
    void tickItems();

    void tickInventories();

    Game* game;
    Scene* scene;
    EntityLayer* entityLayer;
};