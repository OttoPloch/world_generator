#pragma once

#include "../core/common.hpp"
#include <SFML/Graphics/Rect.hpp>

struct BackgroundObject
{
    BackgroundObject();

    BackgroundObject(sf::FloatRect rect, sf::FloatRect texCoords);

    sf::FloatRect rect;
    sf::FloatRect texCoords;

    // used to sort all background objects
    float bottom;
};