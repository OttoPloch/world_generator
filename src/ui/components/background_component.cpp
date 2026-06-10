#include "background_component.hpp"
#include "../../graphics/vertex_group.hpp"
#include "../../core/game.hpp"
#include <SFML/Graphics/Texture.hpp>

BackgroundComponent::BackgroundComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, sf::Vector2f size, float borderWidth, sf::Texture* texture, TextureAtlas* atlas, bool includeBorderAsOffset) : UIComponent(game, myElement, position, identifier, sortIndex), size(size), borderSize(borderWidth, borderWidth), texture(texture), atlas(atlas), includeBorderAsOffset(includeBorderAsOffset)
{
    renderStates.texture = texture;

    updateVisuals();
}

sf::FloatRect BackgroundComponent::getLocalBounds()
{
    return {position.position + originOffset + anchorOffset - borderSize, effectiveSize};
}

void BackgroundComponent::resize(sf::Vector2f newSize)
{
    // TODO: should this set effectiveSize, meaning the whole size of the component, or just the size of the center area?
    size = newSize;

    updateVisuals();
}

void BackgroundComponent::updateVisuals()
{
    effectiveSize = {borderSize.x * 2 + size.x, borderSize.y * 2 + size.y};

    originOffset = borderOffset + UIPosition::getOriginOffset(position, effectiveSize);
    anchorOffset = UIPosition::getAnchorOffset(position, myElement->getLocalBoundsUpToComponent(sortIndex));

    updateVertices();
}

void BackgroundComponent::draw(bool debug)
{
    game->getWindow()->getWindow().draw(vertices.begin(), vertices.size(), sf::PrimitiveType::Triangles, renderStates);
    
    if (debug)
    {
        game->getWindow()->getWindow().draw(debugVertices.begin(), debugVertices.size(), sf::PrimitiveType::Triangles);
    }
}

void BackgroundComponent::updateVertices()
{
    // TODO, probably make this more efficient

    if (atlas)
    {
        std::array<sf::Vertex, 6> centerVerts;
        std::array<sf::Vertex, 6> topLeftVerts;
        std::array<sf::Vertex, 6> topVerts;
        std::array<sf::Vertex, 6> topRightVerts;
        std::array<sf::Vertex, 6> leftVerts;
        std::array<sf::Vertex, 6> rightVerts;
        std::array<sf::Vertex, 6> bottomLeftVerts;
        std::array<sf::Vertex, 6> bottomVerts;
        std::array<sf::Vertex, 6> bottomRightVerts;

        if (includeBorderAsOffset) borderOffset = borderSize;
        else borderOffset = {0, 0};

        centerVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset, size, atlas->itemTexCoords["center"]);
        topLeftVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - borderSize, borderSize, atlas->itemTexCoords["topleft"]);
        topVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(0, borderSize.y), {size.x, borderSize.y}, atlas->itemTexCoords["top"]);
        topRightVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(-size.x, borderSize.y), borderSize, atlas->itemTexCoords["topright"]);
        leftVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(borderSize.x, 0), {borderSize.x, size.y}, atlas->itemTexCoords["left"]);
        rightVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(-size.x, 0), {borderSize.x, size.y}, atlas->itemTexCoords["right"]);
        bottomLeftVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(borderSize.x, -size.y), borderSize, atlas->itemTexCoords["bottomleft"]);
        bottomVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(0, -size.y), {size.x, borderSize.y}, atlas->itemTexCoords["bottom"]);
        bottomRightVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(-size.x, -size.y), borderSize, atlas->itemTexCoords["bottomright"]);
    
        std::array<std::array<sf::Vertex, 6>*, 9> vertsInOrder = {&centerVerts, &topLeftVerts, &topVerts, &topRightVerts, &leftVerts, &rightVerts, &bottomLeftVerts, &bottomVerts, &bottomRightVerts};
        int indexOffset = 0;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                vertices[indexOffset + j] = (*vertsInOrder[i])[j];
            }
    
            indexOffset += 6;
        }
    
        // rest is debug
    
        float alpha = 70;
        sf::Color borderColor(0, 0, 0, alpha);
        sf::Color tlColor(190, 0, 0, alpha);
        sf::Color trColor(150, 150, 0, alpha);
        sf::Color blColor(0, 195, 0, alpha);
        sf::Color brColor(60, 60, 255, alpha);
    
        topLeftVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - borderSize, borderSize, tlColor);
        topVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(0, borderSize.y), {size.x, borderSize.y}, borderColor);
        topRightVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(-size.x, borderSize.y), borderSize, trColor);
        leftVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(borderSize.x, 0), {borderSize.x, size.y}, borderColor);
        rightVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(-size.x, 0), {borderSize.x, size.y}, borderColor);
        bottomLeftVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(borderSize.x, -size.y), borderSize, blColor);
        bottomVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(0, -size.y), {size.x, borderSize.y}, borderColor);
        bottomRightVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(-size.x, -size.y), borderSize, brColor);
    
        vertsInOrder = {&topLeftVerts, &topVerts, &topRightVerts, &leftVerts, &rightVerts, &bottomLeftVerts, &bottomVerts, &bottomRightVerts};

        indexOffset = 0;
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                debugVertices[indexOffset + j] = (*vertsInOrder[i])[j];
            }
    
            indexOffset += 6;
        }
    }
    else
    {
        // TODO: support no atlas
    }
}