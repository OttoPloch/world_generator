#pragma once

#include "ui_component.hpp"

struct BackgroundComponent : public UIComponent
{
    BackgroundComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, sf::Vector2f size, sf::Color backgroundColor = {0, 0, 0});
    
    sf::FloatRect getLocalBounds() override;
    
    void updateVisuals() override;

    void draw() override;
    
    void updateVertices();

    sf::Vector2f size;
    
    sf::Color backgroundColor;

    std::array<sf::Vertex, 6> vertices;
};