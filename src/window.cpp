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
    sf::View newView = sf::View(
        window.getView().getCenter(),
        {static_cast<float>(window.getSize().x),
        static_cast<float>(window.getSize().y)}
    );

    window.setView(newView);
}

sf::RenderWindow& Window::getWindow() { return window; }