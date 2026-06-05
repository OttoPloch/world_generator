#include "background_component.hpp"
#include "../../graphics/vertex_group.hpp"
#include "../../core/game.hpp"

BackgroundComponent::BackgroundComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, sf::Vector2f size, sf::Color backgroundColor) : UIComponent(game, myElement, position, identifier, sortIndex), size(size), backgroundColor(backgroundColor)
{
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

void BackgroundComponent::draw()
{
    game->getWindow()->getWindow().draw(vertices.begin(), vertices.size(), sf::PrimitiveType::Triangles);
}

void BackgroundComponent::updateVertices()
{
    // TODO, make this more efficient

    std::array<sf::Vertex, 6> centerVerts;
    std::array<sf::Vertex, 6> topLeftVerts;
    std::array<sf::Vertex, 6> topVerts;
    std::array<sf::Vertex, 6> topRightVerts;
    std::array<sf::Vertex, 6> leftVerts;
    std::array<sf::Vertex, 6> rightVerts;
    std::array<sf::Vertex, 6> bottomLeftVerts;
    std::array<sf::Vertex, 6> bottomVerts;
    std::array<sf::Vertex, 6> bottomRightVerts;

    // TEMP
    float borderSize = 10;
    sf::Color borderColor(0, 0, 189, 90);
    sf::Color tlColor(190, 0, 0, 90);
    sf::Color trColor(150, 150, 0, 90);
    sf::Color blColor(0, 195, 0, 90);
    sf::Color brColor(90, 90, 255, 90);

    borderOffset = {borderSize, borderSize};
    effectiveSize = {borderSize * 2 + size.x, borderSize * 2 + size.y};

    centerVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset, size, backgroundColor);
    topLeftVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(borderSize, borderSize), {borderSize, borderSize}, tlColor);
    topVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(0, borderSize), {size.x, borderSize}, borderColor);
    topRightVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(-size.x, borderSize), {borderSize, borderSize}, trColor);
    leftVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(borderSize, 0), {borderSize, size.y}, borderColor);
    rightVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(-size.x, 0), {borderSize, size.y}, borderColor);
    bottomLeftVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(borderSize, -size.y), {borderSize, borderSize}, blColor);
    bottomVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(0, -size.y), {size.x, borderSize}, borderColor);
    bottomRightVerts = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset - sf::Vector2f(-size.x, -size.y), {borderSize, borderSize}, brColor);

    std::array<std::array<sf::Vertex, 6>*, 9> vertsInOrder = {&centerVerts, &topLeftVerts, &topVerts, &topRightVerts, &leftVerts, &rightVerts, &bottomLeftVerts, &bottomVerts, &bottomRightVerts};
    int index = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            vertices[index + j] = (*vertsInOrder[i])[j];
        }

        index += 6;
    }
}