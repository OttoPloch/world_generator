#include "vertex_group.hpp"
#include <SFML/Graphics/Rect.hpp>

VertexGroup::VertexGroup() {}

VertexGroup::VertexGroup(int start, int size, sf::IntRect texCoords) : start(start), size(size), texCoords(texCoords) {}

std::array<sf::Vertex, 6> VertexGroup::createTriangleVerts(sf::Vector2f tl, sf::Vector2f size, sf::IntRect texCoords)
{
    std::array<sf::Vertex, 6> vertices;

    vertices[0].position = tl;                                  // tl
    vertices[1].position = {tl.x + size.x, tl.y};          // tr
    vertices[2].position = {tl.x + size.x, tl.y + size.y}; // br
    vertices[3].position = {tl.x, tl.y + size.y};          // bl
    vertices[4].position = vertices[0].position;                // tl
    vertices[5].position = vertices[2].position;                // br

    vertices[0].texCoords = {toFloat(texCoords.position.x), toFloat(texCoords.position.y)};                                                                      // tl
    vertices[1].texCoords = {toFloat(texCoords.position.x + texCoords.size.x), toFloat(texCoords.position.y)};                    // tr                
    vertices[2].texCoords = {toFloat(texCoords.position.x + texCoords.size.x), toFloat(texCoords.position.y + texCoords.size.y)}; // br                                
    vertices[3].texCoords = {toFloat(texCoords.position.x), toFloat(texCoords.position.y + texCoords.size.y)};                    // bl                
    vertices[4].texCoords = vertices[0].texCoords;                                                                   // tl
    vertices[5].texCoords = vertices[2].texCoords;                                                                   // br
    
    return vertices;
}

std::array<sf::Vertex, 8> VertexGroup::createLineVerts(sf::Vector2f tl, sf::Vector2f size, sf::Color color)
{
    std::array<sf::Vertex, 8> vertices;

    vertices[0].position = tl;
    vertices[1].position = {tl.x + size.x, tl.y};
    vertices[2].position = vertices[1].position;
    vertices[3].position = {tl.x + size.x, tl.y + size.y};
    vertices[4].position = vertices[3].position;
    vertices[5].position = {tl.x, tl.y + size.y};
    vertices[6].position = vertices[5].position;
    vertices[7].position = tl;

    for (int i = 0; i < vertices.size(); i++) vertices[i].color = color;

    return vertices;
}