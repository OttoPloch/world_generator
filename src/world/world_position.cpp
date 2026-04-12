#include "world_position.hpp"
#include "../ui/screen_position.hpp"
#include "../core/window.hpp"
#include "../utils/utils.hpp"

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
    sf::Vector2i screenPos(window->getWindow().mapCoordsToPixel(*position));

    return toV2F(screenPos.x, screenPos.y);
}

sf::Vector2f WorldPosition::getPos() { return *position.get(); }