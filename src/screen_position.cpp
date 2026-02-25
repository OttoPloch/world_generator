#include "screen_position.hpp"
#include "world_position.hpp"
#include "window.hpp"
#include "utils.hpp"

ScreenPosition::ScreenPosition() {}

ScreenPosition::ScreenPosition(sf::Vector2f position)
{
    this->position = std::make_shared<sf::Vector2f>(position);
}

ScreenPosition::ScreenPosition(std::shared_ptr<sf::Vector2f> position)
{
    this->position = position;
}

sf::Vector2f ScreenPosition::toWorldCoords(Window* window)
{
    return window->getWindow().mapPixelToCoords(toV2I(position->x, position->y));
}

sf::Vector2f ScreenPosition::getPos() { return *position.get(); }