#pragma once

#include <SFML/Graphics.hpp>

#include "common.hpp"
#include "camera.hpp"

class Window
{
public:
    Window();

    void create(sf::Vector2u size, std::string name, bool fullscreen, int maxFPS = 0, sf::Color bgColor = sf::Color::Black);

    void update(float dt);

    void clear();

    void draw(sf::Drawable& drawable);

    void display();

    void exit();

    void resetView();

    sf::RenderWindow& getWindow();
private:
    sf::RenderWindow window;

    Camera camera;

    sf::Color bgColor;

    sf::Clock c;
};