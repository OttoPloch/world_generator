#include "ui_element.hpp"
#include "../core/game.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <utility>

UIElement::UIElement(Game* game, std::string name, UIPosition position, int z, UIElement* parent) : game(game), name(name), position(position), z(z), parent(parent)
{
    updateVisuals();
}

sf::FloatRect UIElement::getGlobalBounds()
{
    // relative coordinates
    float left = 0, top = 0, right = 0, bottom = 0;
    
    calculateEffectivePosition();

    for (auto& c : components)
    {
        sf::FloatRect cBox = c->getLocalBounds();

        left = std::min(left, cBox.position.x);
        top = std::min(top, cBox.position.y);
        right = std::max(right, cBox.position.x + cBox.size.x);
        bottom = std::max(bottom, cBox.position.y + cBox.size.y);
    }

    if (right < 0)
    {
        float swap = left;
        left = right;
        right = swap;
    }
    if (bottom < 0)
    {
        float swap = top;
        top = bottom;
        bottom = swap;
    }

    // if (name == "test")
    // {
    //     std::cout << "- \n";
    //     std::cout << effectivePosition.x << ", " << effectivePosition.y << '\n';
    //     std::cout << left << ", " << top << '\n';
    //     std::cout << right << ", " << bottom << '\n'; 
    //     std::cout << "FINAL: " << effectivePosition.x + left << ", " << effectivePosition.y + top << "; " << right - left << ", " << bottom - top << ";\n";
    // }

    // converts to global coordinates
    return {{effectivePosition.x + left, effectivePosition.y + top}, {right - left, bottom - top}};
}

void UIElement::updateVisuals()
{
    anchorOffset = UIPosition::getAnchorOffset(position.anchor, this);

    calculateEffectivePosition();

    for (auto& c : components)
    {
        c->updateVisuals();
    }
}

void UIElement::draw()
{
    for (auto& c : components) c->draw();
}

sf::Vector2f UIElement::calculateEffectivePosition()
{
    if (parent)
    {
        effectivePosition = parent->calculateEffectivePosition() + anchorOffset + position.position;
    }
    else
    {
        effectivePosition = anchorOffset + position.position;
    }

    return effectivePosition;
}