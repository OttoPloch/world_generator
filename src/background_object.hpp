#pragma once

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

struct BackgroundObject
{
    BackgroundObject();

    BackgroundObject(sf::Vector2f center, sf::Vector2f size, sf::Vector2f texCoords, sf::Vector2f texCoordDimensions);

    sf::Vector2f center;
    sf::Vector2f size;
    sf::Vector2f texCoords;
    sf::Vector2f texCoordDimensions;
};