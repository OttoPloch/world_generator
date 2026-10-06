#pragma once

#include <SFML/System/Vector2.hpp>

class UIElement;

enum class UIOrigin
{
    TOP_LEFT,
    TOP,
    TOP_RIGHT,
    LEFT,
    CENTER,
    RIGHT,
    BOTTOM_LEFT,
    BOTTOM,
    BOTTOM_RIGHT,
    
    COUNT
};

enum class UIAnchor
{
    TOP_LEFT,
    TOP,
    TOP_RIGHT,
    LEFT,
    CENTER,
    RIGHT,
    BOTTOM_LEFT,
    BOTTOM,
    BOTTOM_RIGHT,
    
    COUNT
};

struct UIPosition
{
    UIPosition();

    UIPosition(sf::Vector2f p_offset, UIOrigin p_origin = UIOrigin::TOP_LEFT, UIAnchor p_anchor = UIAnchor::TOP_LEFT);

    void setOriginOffset(UIElement* p_element);

    void setAnchorOffset(UIElement* p_element);
    
    sf::Vector2f m_offset;

    UIOrigin m_origin;
    UIAnchor m_anchor;

    sf::Vector2f m_originOffset;
    sf::Vector2f m_anchorOffset;
};