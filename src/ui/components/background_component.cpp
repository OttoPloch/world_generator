#include "background_component.hpp"
#include "../../graphics/vertex_group.hpp"
#include "../../core/game.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>

BackgroundComponent::BackgroundComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, sf::Vector2f size, sf::Color backgroundColor) : UIComponent(game, myElement, position, identifier), size(size), backgroundColor(backgroundColor)
{
    updateVisuals();
}

sf::FloatRect BackgroundComponent::getLocalBounds()
{
    return {position.position, size};
}

void BackgroundComponent::updateVisuals()
{
    updateVertices();
}

void BackgroundComponent::draw()
{
    game->getWindow()->getWindow().draw(vertices.begin(), vertices.size(), sf::PrimitiveType::Triangles);
}

void BackgroundComponent::updateVertices()
{
    vertices = VertexGroup::createTriangleVerts(myElement->position.position + position.position, size, backgroundColor);
}