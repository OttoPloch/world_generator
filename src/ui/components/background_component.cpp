#include "background_component.hpp"
#include "../../graphics/vertex_group.hpp"
#include "../../core/game.hpp"
#include <SFML/Graphics/Texture.hpp>

BackgroundComponent::BackgroundComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, sf::Vector2f size, float borderWidth, sf::Texture* texture, TextureAtlas* atlas) : UIComponent(game, myElement, position, identifier, sortIndex), size(size), borderSize(borderWidth, borderWidth), texture(texture), atlas(atlas)
{
    renderStates.texture = texture;

    updateVisuals();
}

sf::FloatRect BackgroundComponent::getLocalBounds()
{
    return {position.position + originOffset + anchorOffset - borderOffset, effectiveSize};
}

void BackgroundComponent::updateVisuals()
{
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
    // TODO, make this more efficient

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
    
        borderOffset = borderSize;
        effectiveSize = {borderSize.x * 2 + size.x, borderSize.y * 2 + size.y};
    
        centerVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset, size, atlas->getItemTexCoords("center"));
        topLeftVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - borderSize, borderSize, atlas->getItemTexCoords("topleft"));
        topVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(0, borderSize.y), {size.x, borderSize.y}, atlas->getItemTexCoords("top"));
        topRightVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(-size.x, borderSize.y), borderSize, atlas->getItemTexCoords("topright"));
        leftVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(borderSize.x, 0), {borderSize.x, size.y}, atlas->getItemTexCoords("left"));
        rightVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(-size.x, 0), {borderSize.x, size.y}, atlas->getItemTexCoords("right"));
        bottomLeftVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(borderSize.x, -size.y), borderSize, atlas->getItemTexCoords("bottomleft"));
        bottomVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(0, -size.y), {size.x, borderSize.y}, atlas->getItemTexCoords("bottom"));
        bottomRightVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(-size.x, -size.y), borderSize, atlas->getItemTexCoords("bottomright"));
    
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
        sf::Color centerColor(35, 35, 35, alpha);
        sf::Color borderColor(0, 0, 0, alpha);
        sf::Color tlColor(190, 0, 0, alpha);
        sf::Color trColor(150, 150, 0, alpha);
        sf::Color blColor(0, 195, 0, alpha);
        sf::Color brColor(60, 60, 255, alpha);
    
        centerVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset, size, centerColor);
        topLeftVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - borderSize, borderSize, tlColor);
        topVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(0, borderSize.y), {size.x, borderSize.y}, borderColor);
        topRightVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(-size.x, borderSize.y), borderSize, trColor);
        leftVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(borderSize.x, 0), {borderSize.x, size.y}, borderColor);
        rightVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(-size.x, 0), {borderSize.x, size.y}, borderColor);
        bottomLeftVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(borderSize.x, -size.y), borderSize, blColor);
        bottomVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(0, -size.y), {size.x, borderSize.y}, borderColor);
        bottomRightVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(-size.x, -size.y), borderSize, brColor);
    
        indexOffset = 0;
        for (int i = 0; i < 9; i++)
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

    }
}