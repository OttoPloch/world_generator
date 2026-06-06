#pragma once

#include "ui_component.hpp"
#include "../../graphics/texture_atlas.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>

struct ButtonComponent : public UIComponent
{
    ButtonComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, sf::Texture* buttonTexture, TextureAtlas* buttonTextureAtlas, sf::Vector2f size = {1, 1}, bool sizeIsScale = true);
    
    sf::FloatRect getLocalBounds() override;
    
    void updateVisuals() override;

    void update(float dt) override;

    void draw(bool debug) override;
    
    // sets the size, texture and atlas of the button. Leave either blank to keep it the same as before.
    void setButtonVisuals(sf::Vector2f size, bool sizeIsScale = true, sf::Texture* newTexture = nullptr, TextureAtlas* newAtlas = nullptr);
private:
    void updateVertices();

    sf::Texture* buttonTexture;
    TextureAtlas* buttonTextureAtlas;
    int textureScale;
    sf::Vector2f size;

    std::array<sf::Vertex, 6> vertices;
    sf::FloatRect texCoords;

    sf::RenderStates renderStates;
};