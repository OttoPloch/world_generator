#pragma once

#include "common.hpp"
#include <SFML/Graphics/Rect.hpp>

struct VertexGroup
{
    sf::IntRect texCoords;

    VertexGroup();

    VertexGroup(sf::IntRect texCoords);

    static std::array<sf::Vertex, 6> createTriangleVerts(sf::Vector2f tl, sf::Vector2f size, sf::IntRect texCoords);
    
    static std::array<sf::Vertex, 8> createLineVerts(sf::Vector2f tl, sf::Vector2f size, sf::Color color);
};