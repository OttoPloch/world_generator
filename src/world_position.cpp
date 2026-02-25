#include "world_position.hpp"
#include "screen_position.hpp"
#include "window.hpp"
#include "utils.hpp"

WorldPosition::WorldPosition() {}

WorldPosition::WorldPosition(sf::Vector2f position)
{
    this->position = std::make_shared<sf::Vector2f>(position);
}

WorldPosition::WorldPosition(std::shared_ptr<sf::Vector2f> position)
{
    this->position = position;
}

sf::Vector2f WorldPosition::toScreenPosition(Window* window)
{
    sf::Vector2i screenPos(window->getWindow().mapCoordsToPixel(*position.get()));

    return toV2F(screenPos.x, screenPos.y);
}

sf::Vector2f WorldPosition::getPos() { return *position.get(); }