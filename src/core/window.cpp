#include "window.hpp"
#include <SFML/Window/VideoMode.hpp>

Window::Window() {}

void Window::create(sf::Vector2u size, std::string name, bool fullscreen, int maxFPS, sf::Color bgColor)
{
    sf::State windowState;

    if (fullscreen)
    {
        windowState = sf::State::Fullscreen;
        size = sf::VideoMode::getFullscreenModes()[0].size;
    }
    else
    {
        windowState = sf::State::Windowed;
    }

    window.create(sf::VideoMode(size), name, windowState);

    if (maxFPS > 0)
    {
        window.setFramerateLimit(maxFPS);
    }
    
    this->bgColor = bgColor;
}

void Window::clear()
{
    window.clear(bgColor);
}

void Window::draw(sf::Drawable& drawable)
{
    window.draw(drawable);
}

void Window::display()
{
    window.display();
}

void Window::exit()
{
    window.close();
}

void Window::setView(sf::View view)
{
    window.setView(view);
}

sf::RenderWindow& Window::getWindow() { return window; }

sf::Vector2u Window::getSize() { return window.getSize(); }