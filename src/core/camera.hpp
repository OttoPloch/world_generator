#pragma once

#include "common.hpp"
#include "../entities/entity.hpp"

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

    sf::Vector2f getSize();

    void tick();

    void update(float dt);

    void setVelocity(sf::Vector2f newVelocity);

    // 'x' or 'y'
    void setVelocity(char direction, float newVelocity);
    
    void changeVelocity(sf::Vector2f amount);
    
    // 'x' or 'y'
    void changeVelocity(char direction, float amount);

    void zoom(int amount);

    void resetZoom();

    void windowResized(sf::Vector2u oldSize, sf::Vector2u newSize);

    void setBaseSize(sf::Vector2f newSize);

    void setFocus(Entity* newFocus);

    void removeFocus();

    Entity* getFocus();
private:
    void updatePosition();

    void updateZoom();

    Game* game;

    Window* window;

    sf::Vector2f baseSize;
    
    sf::Vector2f size;

    float defaultZoom;
    float zoomSpeed;
    float zoomAmount;

    sf::Vector2f center;
    sf::Vector2f velocity;

    sf::View view;

    Entity* focus;
};