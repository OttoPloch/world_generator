#include "ui_component.hpp"
#include "../ui_element.hpp"
#include <SFML/Graphics/Rect.hpp>

UIComponent::UIComponent(Game* game, UIElement* myElement, UIPosition position) : game(game), myElement(myElement), position(position) {}

sf::FloatRect UIComponent::getLocalBounds()
{
    return {position.position, {0, 0}};
}

void UIComponent::updateVisuals() {}

void UIComponent::draw() {}