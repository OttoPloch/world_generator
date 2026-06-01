#include "ui_element.hpp"
#include "../core/game.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <utility>
#include <algorithm>

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

    // converts to global coordinates
    return {{effectivePosition.x + left, effectivePosition.y + top}, {right - left, bottom - top}};
}

std::vector<sf::FloatRect> UIElement::getAllComponentBounds()
{
    std::vector<sf::FloatRect> bounds;

    for (auto& c : components)
    {
        bounds.emplace_back(c->getGlobalBounds());
    }

    return bounds;
}

sf::FloatRect UIElement::getLocalBoundsUpToComponent(int sortIndex)
{
    float left = 0, top = 0, right = 0, bottom = 0;

    calculateEffectivePosition();

    for (auto& c : components)
    {
        if (c->sortIndex >= sortIndex) break;

        sf::FloatRect cBox = c->getLocalBounds();

        left = std::min(left, cBox.position.x);
        top = std::min(top, cBox.position.y);
        right = std::max(right, cBox.position.x + cBox.size.x);
        bottom = std::max(bottom, cBox.position.y + cBox.size.y);
    }

    return {{left, top}, {right - left, bottom - top}};
}

void UIElement::updateVisuals()
{
    anchorOffset = UIPosition::getAnchorOffset(position, this);

    calculateEffectivePosition();

    for (auto& c : components)
    {
        c->updateVisuals();
    }
}

void UIElement::update(float dt)
{
    for (auto& c : components) c->update(dt);
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

void UIElement::sortComponents()
{
    std::sort(components.begin(), components.end(), [](const std::unique_ptr<UIComponent>& a, const std::unique_ptr<UIComponent>& b){
        return a->sortIndex < b->sortIndex;
    });
}