#pragma once

#include "ui_component.hpp"
#include <SFML/Graphics/RenderStates.hpp>

struct ButtonComponent : public UIComponent
{
    ButtonComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, sf::Texture* texture, int textureScale = 1);
    
    sf::FloatRect getLocalBounds() override;
    
    void updateVisuals() override;

    void draw() override;
    
    void setTexture(sf::Texture* newTexture);

    bool isSelected();
    
    bool isPressed();
private:
    void updateVertices();
    
    sf::Texture* texture;
    int textureScale;
    sf::Vector2f size;

    std::array<sf::Vertex, 6> vertices;

    sf::RenderStates states;
};