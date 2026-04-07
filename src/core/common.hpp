#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <cmath>

#include "input.hpp"
#include "../utils/conversions.hpp"
#include "../utils/utils.hpp"

struct Vector2iHash
{
    std::size_t operator()(const sf::Vector2i& v) const noexcept
    {
        // Simple hash combine
        std::size_t h1 = std::hash<int>()(v.x);
        std::size_t h2 = std::hash<int>()(v.y);
        return h1 ^ (h2 << 1);
    }
};