#include "window.hpp"
#include <SFML/Window/VideoMode.hpp>

Window::Window() {}

void Window::create(sf::Vector2u size, std::string name, bool fullscreen, int maxFPS, sf::Color bgColor)
{
    sf::State windowState;

    windowedSize = size;
    this->name = name;
    this->fullscreen = fullscreen;
    this->maxFPS = maxFPS;

    sf::VideoMode vm;
    if (fullscreen)
    {
        windowState = sf::State::Fullscreen;
        vm = sf::VideoMode::getDesktopMode();
    }
    else
    {
        windowState = sf::State::Windowed;
        vm = sf::VideoMode(windowedSize);
    }

    window.create(vm, name, windowState);

    window.setMouseCursorVisible(false);

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

void Window::toggleFullscreen()
{
    fullscreen = !fullscreen;

    create(windowedSize, name, fullscreen, maxFPS, bgColor);
}

void Window::resized(sf::View newView)
{
    if (!fullscreen) windowedSize = window.getSize();

    setView(newView);
}

void Window::setView(sf::View view)
{
    window.setView(view);
}

sf::RenderWindow& Window::getWindow() { return window; }

sf::Vector2u Window::getSize() { return window.getSize(); }

float Window::getAspectRatio()
{
    return static_cast<float>(window.getSize().x) / static_cast<float>(window.getSize().y);
}