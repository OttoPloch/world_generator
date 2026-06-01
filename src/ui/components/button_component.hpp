#pragma once

#include "ui_component.hpp"
#include "../../graphics/texture_atlas.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>

enum class ButtonState
{
    UP,
    HOVER,
    DOWN,

    COUNT
};

struct ButtonComponent : public UIComponent
{
    ButtonComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, sf::Texture* buttonTexture, TextureAtlas* buttonTextureAtlas, sf::Vector2f size = {1, 1}, bool sizeIsScale = true);
    
    sf::FloatRect getLocalBounds() override;
    
    void updateVisuals() override;

    void update(float dt) override;

    void draw() override;
    
    // sets the size, texture and atlas of the button. Leave either blank to keep it the same as before.
    void setButtonVisuals(sf::Vector2f size, bool sizeIsScale = true, sf::Texture* newTexture = nullptr, TextureAtlas* newAtlas = nullptr);

    // checks if the button is currently hovered on,
    // either by the mouse cursor or the controller selector.
    bool isSelected();
    
    // checks for a full valid button press.
    bool isDown();

    // checks if the button was just pressed this frame,
    // and wasn't pressed last frame.
    bool justPressed();

    std::map<ButtonState, std::string> statesToItemName;
private:
    void updateVertices();
    
    // just checks if button is selected and pressed,
    // not necessarily a valid button press.
    bool isPressed();
    
    // checks if the button was just selected this frame,
    // and wasn't selected last frame.
    bool justSelected();

    sf::Texture* buttonTexture;
    TextureAtlas* buttonTextureAtlas;
    int textureScale;
    sf::Vector2f size;

    std::array<sf::Vertex, 6> vertices;
    sf::FloatRect texCoords;

    sf::RenderStates renderStates;
    ButtonState buttonState;

    bool pressed;
    bool pressedLastFrame;

    // for tracking if the button was selected while
    // the mouse was already clicked, don't want to
    // press if that is the case.
    bool canPress;
    bool selected;
    bool selectedLastFrame;
};