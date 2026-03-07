#include "vertex_group.hpp"
#include <SFML/Graphics/Rect.hpp>

VertexGroup::VertexGroup() {}

VertexGroup::VertexGroup(int start, int size, sf::FloatRect texCoords) : start(start), size(size), texCoords(texCoords) {}

std::array<sf::Vertex, 6> VertexGroup::createVerts(sf::Vector2f tl, sf::Vector2f size, sf::FloatRect texCoords)
{
    std::array<sf::Vertex, 6> vertices;

    vertices[0].position = tl;                                  // tl
    vertices[1].position = {tl.x + size.x, tl.y};          // tr
    vertices[2].position = {tl.x + size.x, tl.y + size.y}; // br
    vertices[3].position = {tl.x, tl.y + size.y};          // bl
    vertices[4].position = vertices[0].position;                // tl
    vertices[5].position = vertices[2].position;                // br

    vertices[0].texCoords = texCoords.position;                                                                      // tl
    vertices[1].texCoords = {texCoords.position.x + texCoords.size.x, texCoords.position.y};                    // tr                
    vertices[2].texCoords = {texCoords.position.x + texCoords.size.x, texCoords.position.y + texCoords.size.y}; // br                                
    vertices[3].texCoords = {texCoords.position.x, texCoords.position.y + texCoords.size.y};                    // bl                
    vertices[4].texCoords = vertices[0].texCoords;                                                                   // tl
    vertices[5].texCoords = vertices[2].texCoords;                                                                   // br
    
    return vertices;
}
