#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "input.hpp"
#include "conversions.hpp"
#include "utils.hpp"

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