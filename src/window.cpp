#include "window.hpp"

Window::Window() {}

void Window::create(sf::Vector2u size, std::string name, bool fullscreen, int maxFPS, sf::Color bgColor)
{
    sf::State windowState;

    (fullscreen) ? windowState = sf::State::Fullscreen : windowState = sf::State::Windowed;

    window.create(sf::VideoMode(size), name, windowState);

    if (maxFPS > 0)
    {
        window.setFramerateLimit(maxFPS);
    }

    this->bgColor = bgColor;

    camera.init(*this, true, {0, 0}, {static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});

    window.setView(camera.getView());
}

void Window::update(float dt)
{
    window.setView(camera.getView());
    
    camera.update(dt);

    if (getKey("W"))
    {
        camera.setMovement('y', -1);
    }
    
    if (getKey("A"))
    {
        camera.setMovement('x', -1);
    }
    
    if (getKey("S"))
    {
        camera.setMovement('y', 1);
    }
    
    if (getKey("D"))
    {
        camera.setMovement('x', 1);
    }
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

void Window::resetView()
{
    camera.setSize({
        static_cast<float>(window.getSize().x),
        static_cast<float>(window.getSize().y)
    });
}

sf::RenderWindow& Window::getWindow() { return window; }