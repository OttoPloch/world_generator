#pragma once

#include "ui_component.hpp"
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
    ButtonComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, std::map<ButtonState, sf::Texture*> textures, sf::Vector2f size = {1, 1}, bool sizeIsScale = true);
    
    sf::FloatRect getLocalBounds() override;
    
    void updateVisuals() override;

    void update(float dt) override;

    void draw() override;
    
    // change the textures of the button's different states.
    void setButtonTextures(std::map<ButtonState, sf::Texture*> newTextures, sf::Vector2f size = {1, 1}, bool sizeIsScale = true);

    // checks if the button is currently hovered on,
    // either by the mouse cursor or the controller selector.
    bool isSelected();
    
    // checks for a full valid button press.
    bool isDown();

    // checks if the button was just pressed this frame,
    // and wasn't pressed last frame.
    bool justPressed();

private:
    void updateVertices();
    
    // just checks if button is selected and pressed,
    // not necessarily a valid button press.
    bool isPressed();
    
    // checks if the button was just selected this frame,
    // and wasn't selected last frame.
    bool justSelected();

    std::map<ButtonState, sf::Texture*> textures;
    int textureScale;
    sf::Vector2f size;

    std::array<sf::Vertex, 6> vertices;

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