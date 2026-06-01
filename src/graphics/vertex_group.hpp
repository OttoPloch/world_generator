#pragma once

#include "../core/common.hpp"
#include <SFML/Graphics/Rect.hpp>

struct VertexGroup
{
    sf::FloatRect texCoords;

    VertexGroup();

    VertexGroup(sf::FloatRect texCoords);

    static std::array<sf::Vertex, 6> createTriangleVerts(sf::Vector2f tl, sf::Vector2f size, sf::FloatRect texCoords);
    
    static std::array<sf::Vertex, 6> createTriangleVerts(sf::Vector2f tl, sf::Vector2f size, sf::Color color);
    
    static std::array<sf::Vertex, 8> createLineVerts(sf::Vector2f tl, sf::Vector2f size, sf::Color color);
};