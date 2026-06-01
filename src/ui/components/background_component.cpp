#include "background_component.hpp"
#include "../../graphics/vertex_group.hpp"
#include "../../core/game.hpp"

BackgroundComponent::BackgroundComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, sf::Vector2f size, sf::Color backgroundColor) : UIComponent(game, myElement, position, identifier, sortIndex), size(size), backgroundColor(backgroundColor)
{
    updateVisuals();
}

sf::FloatRect BackgroundComponent::getLocalBounds()
{
    return {position.position + originOffset + anchorOffset, size};
}

void BackgroundComponent::updateVisuals()
{
    originOffset = UIPosition::getOriginOffset(position, size);
    anchorOffset = UIPosition::getAnchorOffset(position, myElement->getLocalBoundsUpToComponent(sortIndex));

    updateVertices();
}

void BackgroundComponent::draw()
{
    game->getWindow()->getWindow().draw(vertices.begin(), vertices.size(), sf::PrimitiveType::Triangles);
}

void BackgroundComponent::updateVertices()
{
    vertices = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset, size, backgroundColor);
}