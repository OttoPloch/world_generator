#include "event_handler.hpp"

#include "game.hpp"

EventHandler::EventHandler() {}

void EventHandler::init(Window* window, Camera* camera, Game* game, Scene* scene)
{
    this->window = window;

    this->camera = camera;

    this->game = game;

    this->scene = scene;
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
            windowResized();
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

void EventHandler::windowResized()
{
    camera->setBaseSize(toV2F(window->getSize()));
    window->getWindow().setView(camera->getView());
    scene->getUILayer()->reset();
}