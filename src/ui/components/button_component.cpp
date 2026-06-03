#include "button_component.hpp"
#include "../../graphics/vertex_group.hpp"
#include "../../core/game.hpp"
#include <SFML/Graphics/CoordinateType.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/StencilMode.hpp>

ButtonComponent::ButtonComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, sf::Texture* buttonTexture, TextureAtlas* buttonTextureAtlas, sf::Vector2f size, bool sizeIsScale) : UIComponent(game, myElement, position, identifier, sortIndex)
{
    setButtonVisuals(size, sizeIsScale, buttonTexture, buttonTextureAtlas);
}

sf::FloatRect ButtonComponent::getLocalBounds()
{
    return {position.position + originOffset + anchorOffset, size};
}

void ButtonComponent::updateVisuals()
{
    originOffset = UIPosition::getOriginOffset(position, size);
    anchorOffset = UIPosition::getAnchorOffset(position, myElement->getLocalBoundsUpToComponent(sortIndex));

    updateVertices();
}

void ButtonComponent::update(float dt)
{   
    updateState();
}

void ButtonComponent::draw()
{
    game->getWindow()->getWindow().draw(vertices.data(), vertices.size(), sf::PrimitiveType::Triangles, renderStates.texture);
}

void ButtonComponent::setButtonVisuals(sf::Vector2f newSize, bool sizeIsScale, sf::Texture* newTexture, TextureAtlas* newAtlas)
{
    if (newTexture) buttonTexture = newTexture;
    if (newAtlas) buttonTextureAtlas = newAtlas;

    renderStates.texture = buttonTexture;

    texCoords = buttonTextureAtlas->itemTexCoords[statesToItemName[uiState]];

    size = texCoords.size;

    if (sizeIsScale)
    {
        size.x *= newSize.x;
        size.y *= newSize.y;
    }
    else
    {
        size = newSize;
    }

    updateVisuals();
}

void ButtonComponent::updateVertices()
{
    vertices = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset, size, buttonTextureAtlas->itemTexCoords[statesToItemName[uiState]]);
}