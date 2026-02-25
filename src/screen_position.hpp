#pragma once

#include <SFML/System/Vector2.hpp>
#include <memory>

struct WorldPosition;
class Window;

struct ScreenPosition
{
    ScreenPosition();

    ScreenPosition(sf::Vector2f position);

    ScreenPosition(std::shared_ptr<sf::Vector2f> position);

    sf::Vector2f toWorldCoords(Window* window);

    sf::Vector2f getPos();

    std::shared_ptr<sf::Vector2f> position;
};