#pragma once

#include <SFML/System/Vector2.hpp>
#include <memory>

struct ScreenPosition;
class Window;

struct WorldPosition
{
    WorldPosition();

    WorldPosition(sf::Vector2<double> position);

    WorldPosition(std::shared_ptr<sf::Vector2<double>> position);
    
    sf::Vector2f toScreenPosition(Window* window);

    sf::Vector2<double> getPos();

    std::shared_ptr<sf::Vector2<double>> position;
};