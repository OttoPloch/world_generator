#include "ui_element.hpp"
#include "../core/game.hpp"
#include <SFML/Graphics/Rect.hpp>

UIElement::UIElement(Game* game, std::string name, UIPosition position, int z) : game(game), name(name), position(position), z(z) {}

sf::FloatRect UIElement::getGlobalBounds()
{
    // relative coordinates
    float left = 0, top = 0;
    sf::Vector2f size(0, 0);
    
    for (auto& c : components)
    {
        sf::FloatRect cBox = c->getLocalBounds();

        left = std::min(left, cBox.position.x);
        top = std::min(top, cBox.position.y);
        size.x = std::max(size.x, cBox.size.x);
        size.y = std::max(size.y, cBox.size.y);
    }
    
    // converts to global coordinates
    return {{position.position.x + left, position.position.y + top}, size};
}

void UIElement::updateVisuals()
{
    for (auto& c : components)
    {
        c->updateVisuals();
    }
}

void UIElement::draw()
{
    for (auto& c : components) c->draw();
}