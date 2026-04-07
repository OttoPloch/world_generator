#pragma once

#include "common.hpp"
#include "camera.hpp"

class Window
{
public:
    Window();

    void create(sf::Vector2u size, std::string name, bool fullscreen, int maxFPS = 0, sf::Color bgColor = sf::Color::Black);

    void clear();

    void draw(sf::Drawable& drawable);

    void display();

    void exit();

    void setView(sf::View view);

    sf::RenderWindow& getWindow();

    sf::Vector2u getSize();
private:
    sf::RenderWindow window;

    sf::Color bgColor;
};