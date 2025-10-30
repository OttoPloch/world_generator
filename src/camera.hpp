#pragma once

#include "common.hpp"

class Window;

class Camera
{
public:
    Camera();

    void init(Window& window, bool setTopLeftPos, sf::Vector2f position, sf::Vector2f size);

    void setCenter(sf::Vector2f center);

    void setTopLeft(sf::Vector2f topLeft);

    sf::View getView();

    sf::Vector2f getCenter();

    sf::Vector2f getTopLeft();

    float getZoomFactor();

    void update(float dt);

    void setVelocity(sf::Vector2f newVelocity);

    void setVelocity(char direction, float newVelocity);
    
    void changeVelocity(sf::Vector2f amount);

    void changeVelocity(char direction, float amount);

    void zoom(int amount);

    void resetZoom();

    void setBaseSize(sf::Vector2f newSize);
private:
    Window* window;

    sf::Vector2f baseSize;

    sf::Vector2f size;
    
    float zoomFactor;

    sf::Vector2f center;

    sf::Vector2f velocity;

    sf::View view;
};