#include "ui_component.hpp"
#include "../ui_element.hpp"
#include <SFML/Graphics/Rect.hpp>

UIComponent::UIComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex) : game(game), myElement(myElement), position(position), identifier(identifier), sortIndex(sortIndex) {}

sf::FloatRect UIComponent::getGlobalBounds()
{
    sf::FloatRect lb = getLocalBounds();

    return {myElement->effectivePosition + lb.position, lb.size};
}

sf::FloatRect UIComponent::getLocalBounds()
{
    return {position.position + originOffset + anchorOffset, {0, 0}};
}

void UIComponent::updateVisuals()
{
    originOffset = {0, 0};
    anchorOffset = {0, 0};
}

void UIComponent::draw() {}