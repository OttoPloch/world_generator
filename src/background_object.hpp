#pragma once

#include "common.hpp"
#include <SFML/Graphics/Rect.hpp>

struct BackgroundObject
{
    BackgroundObject();

    BackgroundObject(sf::FloatRect rect, sf::FloatRect texCoords);

    sf::FloatRect rect;
    sf::FloatRect texCoords;
};