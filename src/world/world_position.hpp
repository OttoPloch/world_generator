#pragma once

#include <SFML/System/Vector2.hpp>
#include <memory>

struct ScreenPosition;
class Window;

struct WorldPosition
{
    WorldPosition();

    WorldPosition(sf::Vector2f position);

    WorldPosition(std::shared_ptr<sf::Vector2f> position);
    
    sf::Vector2f toScreenPosition(Window* window);

    sf::Vector2f getPos();

    std::shared_ptr<sf::Vector2f> position;
};