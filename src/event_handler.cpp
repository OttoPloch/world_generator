#include "event_handler.hpp"

EventHandler::EventHandler() {}

void EventHandler::init(Window& window, Camera& camera)
{
    this->window = &window;

    this->camera = &camera;
}

void EventHandler::processEvents()
{
    while (const auto event = window->getWindow().pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            windowClosed();
        }
        else if (event->is<sf::Event::Resized>())
        {
            windowResized();
        }
        else if (const auto key = event->getIf<sf::Event::KeyPressed>())
        {
            keyPressed(key->code);
        }
        else if (const auto scroll = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            mouseWheelScrolled(scroll);
        }
    }
}

void EventHandler::windowClosed()
{
    window->exit();
}

void EventHandler::windowResized()
{
    camera->setBaseSize(toV2F(window->getSize()));
}

void EventHandler::keyPressed(sf::Keyboard::Key key)
{
    switch(key)
    {
        case sf::Keyboard::Key::Escape:
            windowClosed();
            break;
        case sf::Keyboard::Key::Enter:
            camera->resetZoom();
            break;
    }
}

void EventHandler::mouseWheelScrolled(const sf::Event::MouseWheelScrolled* scroll)
{
    camera->zoom(toInt(-scroll->delta));
}