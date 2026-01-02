#pragma once

#include "common.hpp"
#include "entity.hpp"
#include "gamerules.hpp"

class Game;
class Window;

class Camera
{
public:
    Camera();

    void init(Game* game, bool setTopLeftPos, sf::Vector2f position, sf::Vector2f size, Entity* focus = nullptr);

    void setCenter(sf::Vector2f center);

    void setTopLeft(sf::Vector2f topLeft);

    sf::View getView();

    sf::Vector2f getCenter();

    sf::Vector2f getTopLeft();

    float getZoomFactor();

    void update(float dt);

    void setVelocity(sf::Vector2f newVelocity);

    // 'x' or 'y'
    void setVelocity(char direction, float newVelocity);
    
    void changeVelocity(sf::Vector2f amount);
    
    // 'x' or 'y'
    void changeVelocity(char direction, float amount);

    void zoom(int amount);

    void resetZoom();

    void setBaseSize(sf::Vector2f newSize);

    void setFocus(Entity* newFocus);

    void removeFocus();

    Entity* getFocus();
private:
    Game* game;

    Window* window;

    Gamerules* gamerules;

    sf::Vector2f baseSize;

    sf::Vector2f size;
    
    float zoomFactor;

    sf::Vector2f center;

    sf::Vector2f velocity;

    sf::View view;

    Entity* focus;
};