#pragma once

#include "../ui_position.hpp"

class Game;
class UIElement;

enum class UIState
{
    IDLE,
    HOVER,
    PRESS,

    COUNT
};

struct UIComponent
{
    UIComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex);

    sf::FloatRect getGlobalBounds();

    virtual sf::FloatRect getLocalBounds();

    virtual void onIdle();
    virtual void onHover();
    virtual void onPress();

    // checks if the component is currently hovered on,
    // either by the mouse cursor or the controller selector.
    bool isSelected();
    
    // checks for a full valid press.
    bool isPressed();

    // checks if the component was just pressed this frame,
    // and wasn't pressed last frame.
    bool justPressed();

    virtual void updateVisuals();

    void updateState();

    // include updateState in overrides to support onIdle(), onHover() and onPress()
    virtual void update(float dt);

    virtual void draw();

    Game* game;
    UIElement* myElement;

    UIPosition position;
    sf::Vector2f originOffset;
    sf::Vector2f anchorOffset;

    std::string identifier;

    // UIElement component vector is sorted by this.
    // This is used to calculate the size of the element
    // with every component up to this one (skipping any with
    // the same index). By doing this, components can be aligned with
    // other components by the anchor without having to account for
    // every component, which would place the anchor on the very
    // edge of the element.
    int sortIndex;
    
    std::map<UIState, std::string> statesToItemName;
protected:
    // just checks if component is selected and pressed,
    // not necessarily a valid press.
    bool attemptedPress();
    
    // checks if the componnet was just selected this frame,
    // and wasn't selected last frame.
    bool justSelected();

    UIState uiState;
private:
    bool pressed;
    bool pressedLastFrame;

    // for tracking if the component was selected while
    // the mouse was already clicked, don't want to
    // press if that is the case.
    bool canPress;
    bool selected;
    bool selectedLastFrame;
};