#pragma once

#include "ui_component.hpp"

struct BackgroundComponent : public UIComponent
{
    BackgroundComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, sf::Vector2f size, sf::Color backgroundColor = {0, 0, 0});
    
    sf::FloatRect getLocalBounds() override;
    
    void updateVisuals() override;

    void draw() override;
private:
    void updateVertices();

    // not including the border
    sf::Vector2f size;

    sf::Vector2f effectiveSize;

    // the space between this component's UIPosition position (top left of the center area),
    // and the top left of the border.
    sf::Vector2f borderOffset;
    
    sf::Color backgroundColor;

    std::array<sf::Vertex, 54> vertices;
};