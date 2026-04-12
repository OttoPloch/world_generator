#include "world_position.hpp"
#include "../ui/screen_position.hpp"
#include "../core/window.hpp"
#include "../utils/utils.hpp"

WorldPosition::WorldPosition() {}

WorldPosition::WorldPosition(sf::Vector2<double> position)
{
    this->position = std::make_shared<sf::Vector2<double>>(position);
}

WorldPosition::WorldPosition(std::shared_ptr<sf::Vector2<double>> position)
{
    this->position = position;
}

sf::Vector2f WorldPosition::toScreenPosition(Window* window)
{
    sf::Vector2i screenPos(window->getWindow().mapCoordsToPixel({static_cast<float>(position->x), static_cast<float>(position->y)}));

    return toV2F(screenPos.x, screenPos.y);
}

sf::Vector2<double> WorldPosition::getPos() { return *position.get(); }