#include "ui_element.hpp"
#include "../core/game.hpp"

UIElement::UIElement(Game* game, std::string name, sf::Vector2f position, sf::Vector2f size, int z, sf::Color backgroundColor) : game(game), name(name), position(position), size(size), z(z)
{
    vertices = VertexGroup::createTriangleVerts(position, size, backgroundColor);
}

void UIElement::draw()
{
    game->getWindow()->getWindow().draw(vertices.begin(), vertices.size(), sf::PrimitiveType::Triangles);
}