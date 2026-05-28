#include "button_component.hpp"
#include "../../graphics/vertex_group.hpp"
#include "../../core/game.hpp"
#include <SFML/Graphics/CoordinateType.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/StencilMode.hpp>

ButtonComponent::ButtonComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, sf::Texture* texture, int textureScale) : UIComponent(game, myElement, position, identifier, sortIndex), textureScale(textureScale)
{
    setTexture(texture);
}

sf::FloatRect ButtonComponent::getLocalBounds()
{
    return {position.position + originOffset + anchorOffset, size};
}

void ButtonComponent::updateVisuals()
{
    originOffset = UIPosition::getOriginOffset(position.origin, size);
    anchorOffset = UIPosition::getAnchorOffset(position.anchor, myElement->getLocalBoundsUpToComponent(sortIndex));

    updateVertices();
}

void ButtonComponent::draw()
{
    game->getWindow()->getWindow().draw(vertices.data(), vertices.size(), sf::PrimitiveType::Triangles, states.texture);
}

void ButtonComponent::setTexture(sf::Texture* newTexture)
{
    texture = newTexture;
    states.texture = texture;

    size = toV2F(texture->getSize()) * toFloat(textureScale);

    updateVisuals();
}

bool ButtonComponent::isSelected()
{
    sf::FloatRect gb = getGlobalBounds();

    return mouseRectCollide(game, gb.position, gb.size);
}

bool ButtonComponent::isPressed()
{
    return isSelected() && game->getInput()->isKeyPressed("LEFTCLICK");
}

void ButtonComponent::updateVertices()
{
    vertices = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset, size, sf::IntRect({0, 0}, toV2I(texture->getSize())));
}