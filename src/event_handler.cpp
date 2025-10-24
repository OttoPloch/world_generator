#include "event_handler.hpp"

EventHandler::EventHandler() {}

void EventHandler::init(Window& window)
{
    this->window = &window;
}

void EventHandler::processEvents()
{
    while (const auto event = window->getWindow().pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            windowClosed();
        }
        else if (const auto key = event->getIf<sf::Event::KeyPressed>())
        {
            keyPressed(key->code);
        }
        else if (event->is<sf::Event::Resized>())
        {
            window->resetView();
        }
    }
}

void EventHandler::windowClosed()
{
    window->exit();
}

void EventHandler::keyPressed(sf::Keyboard::Key key)
{
    switch(key)
    {
        case sf::Keyboard::Key::Escape:
            windowClosed();
            break;
    }
}