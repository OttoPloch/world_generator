#pragma once

#include "../core/common.hpp"

enum class UIOrigin
{
    TOP_LEFT,
    TOP_MIDDLE,
    TOP_RIGHT,
    LEFT_MIDDLE,
    CENTER,
    RIGHT_MIDDLE,
    BOTTOM_LEFT,
    BOTTOM_MIDDLE,
    BOTTOM_RIGHT,

    COUNT
};

enum class UIAnchor
{
    TOP_LEFT,
    TOP_MIDDLE,
    TOP_RIGHT,
    LEFT_MIDDLE,
    CENTER,
    RIGHT_MIDDLE,
    BOTTOM_LEFT,
    BOTTOM_MIDDLE,
    BOTTOM_RIGHT,

    COUNT
};

class UIPosition
{
public:
    UIPosition(sf::Vector2f position, UIOrigin origin = UIOrigin::TOP_LEFT, UIAnchor anchor = UIAnchor::TOP_LEFT, bool worldPosition = false);

    // components only
    static sf::Vector2f getOriginOffset(UIPosition position, sf::Vector2f size);
    
    // for elements
    static sf::Vector2f getAnchorOffset(UIElement* element);
    
    // for components
    static sf::Vector2f getAnchorOffset(UIPosition position, sf::FloatRect bounds);

    // UI positions are relative to their element's parent's position.
    sf::Vector2f position;

    // the origin is which part of the component the position sets.
    // Origins currently only affect components, not elements.
    UIOrigin origin;

    // the anchor is where the origin of the element will be placed
    // in its relative space. For example, a component of an element
    // with no parent and an anchor on the top right will place its
    // origin on the top right of the screen, and an element with a
    // parent element and the same anchor will place itself on the
    // top right of that element. Of couse, the position of the
    // component will act as an offset to the anchor.
    // Anchors currently only affect elements, not components.
    UIAnchor anchor;

    // if true, then the element will be drawn at a position in the
    // world, meaning it will move with the camera.
    bool worldPosition;
};