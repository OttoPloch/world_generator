#include "event_handler.hpp"

#include "game.hpp"

EventHandler::EventHandler() {}

void EventHandler::init(Window* window, Camera* camera, Game* game)
{
    this->window = window;

    this->camera = camera;

    this->game = game;
}

void EventHandler::processEvents()
{
    while (const auto event = window->getWindow().pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            game->exit();
        }
        else if (event->is<sf::Event::Resized>())
        {
            camera->setBaseSize(toV2F(window->getSize()));
        }
        else if (const auto key = event->getIf<sf::Event::KeyPressed>())
        {
            game->processInput(key->code);
        }
        else if (const auto scroll = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            mouseWheelScrolled(scroll);
        }
    }
}

void EventHandler::mouseWheelScrolled(const sf::Event::MouseWheelScrolled* scroll)
{
    if (scroll->wheel == sf::Mouse::Wheel::Horizontal)
    {
        camera->zoom(toInt(scroll->delta));
    }
    else
    {
        camera->zoom(toInt(-scroll->delta));
    }
}