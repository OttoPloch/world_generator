#pragma once

#include "../core/common.hpp"
#include <SFML/Graphics/Rect.hpp>

struct BackgroundObject
{
    BackgroundObject();

    BackgroundObject(sf::FloatRect rect, sf::IntRect texCoords);

    sf::FloatRect rect;
    sf::IntRect texCoords;
};