#pragma once

#include "ui_component.hpp"
#include "../../graphics/texture_atlas.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>

struct BackgroundComponent : public UIComponent
{
    BackgroundComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, sf::Vector2f size, float borderWidth, sf::Texture* texture, TextureAtlas* atlas = nullptr);
    
    sf::FloatRect getLocalBounds() override;
    
    void updateVisuals() override;

    void draw(bool debug) override;
private:
    void updateVertices();

    // not including the border
    sf::Vector2f size;
    sf::Vector2f effectiveSize;

    // the space between this component's UIPosition position (top left of the center area),
    // and the top left of the border. Is used in getLocalBounds() and updateVisuals() to
    // account for the border.
    sf::Vector2f borderOffset;
    sf::Vector2f borderSize;

    sf::Texture* texture;
    TextureAtlas* atlas;

    std::array<sf::Vertex, 54> vertices;
    sf::RenderStates renderStates;
    
    std::array<sf::Vertex, 54> debugVertices;
};