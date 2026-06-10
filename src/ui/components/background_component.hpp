#pragma once

#include "ui_component.hpp"
#include "../../graphics/texture_atlas.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>

struct BackgroundComponent : public UIComponent
{
    BackgroundComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, sf::Vector2f size, float borderWidth, sf::Texture* texture, TextureAtlas* atlas = nullptr, bool includeBorderAsOffset = true);
    
    sf::FloatRect getLocalBounds() override;
    
    void resize(sf::Vector2f newSize) override;

    void updateVisuals() override;

    void draw(bool debug) override;

    private:
    void updateVertices();
    
    // size does not include the border
    sf::Vector2f size;
    sf::Vector2f effectiveSize;
    
    // the space between this component's UIPosition position (top left of the center area),
    // and the top left of the border. Is optionally used to set the top left of the border
    // with position instead of the top left of the center area.
    sf::Vector2f borderOffset;
    bool includeBorderAsOffset;
    sf::Vector2f borderSize;
    
    sf::Texture* texture;
    TextureAtlas* atlas;
    
    std::array<sf::Vertex, 54> vertices;
    sf::RenderStates renderStates;
    
    std::array<sf::Vertex, 48> debugVertices;
};