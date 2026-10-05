#include "ui_position.hpp"
#include "ui_element.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include "../core/game.hpp"

UIPosition::UIPosition(sf::Vector2f p_offset, UIOrigin p_origin, UIAnchor p_anchor) : m_offset(p_offset), m_origin(p_origin), m_anchor(p_anchor)
{

}

void UIPosition::setOriginOffset(UIElement* p_element)
{
    if (!p_element) return;
    
    sf::Vector2f l_newOriginOffset = {0, 0};

    UIPosition l_elementPosition = p_element->getUIPosition();
    sf::Vector2f l_elementSize = p_element->getSize();

    switch (l_elementPosition.m_origin)
    {
        case UIOrigin::TOP_LEFT:
            l_newOriginOffset = {0, 0};
            break;
        case UIOrigin::TOP:
            l_newOriginOffset = {-l_elementSize.x / 2.f, 0};
            break;
        case UIOrigin::TOP_RIGHT:
            l_newOriginOffset = {-l_elementSize.x, 0};
            break;
        case UIOrigin::LEFT:
            l_newOriginOffset = {0, -l_elementSize.y / 2.f};
            break;
        case UIOrigin::CENTER:
            l_newOriginOffset = {-l_elementSize.x / 2.f, -l_elementSize.y / 2.f};
            break;
        case UIOrigin::RIGHT:
            l_newOriginOffset = {-l_elementSize.x, -l_elementSize.y / 2.f};
            break;
        case UIOrigin::BOTTOM_LEFT:
            l_newOriginOffset = {0, -l_elementSize.y};
            break;
        case UIOrigin::BOTTOM:
            l_newOriginOffset = {-l_elementSize.x / 2.f, -l_elementSize.y};
            break;
        case UIOrigin::BOTTOM_RIGHT:
            l_newOriginOffset = -l_elementSize;
            break;
        default:
            l_newOriginOffset = {0, 0};
            break;
    }

    m_originOffset = l_newOriginOffset;
}

void UIPosition::setAnchorOffset(UIElement* p_element)
{
    if (!p_element) return;

    sf::Vector2f l_newAnchorOffset = {0, 0};

    UIPosition l_elementPosition = p_element->getUIPosition();
    sf::FloatRect l_elementRelativeSpace;

    UIElement* l_parentElement = p_element->getParent();
    if (l_parentElement)
    {
        l_elementRelativeSpace = l_parentElement->getGlobalBounds();;
    }
    else
    {
        l_elementRelativeSpace = {{0, 0}, p_element->m_game->getScene()->getUILayer()->getUIViewSize()};
    }

    switch (l_elementPosition.m_anchor)
    {
        case UIAnchor::TOP_LEFT:
            l_newAnchorOffset = l_elementRelativeSpace.position;
            break;
        case UIAnchor::TOP:
            l_newAnchorOffset = {l_elementRelativeSpace.position.x + l_elementRelativeSpace.size.x / 2.f, l_elementRelativeSpace.position.y};
            break;
        case UIAnchor::TOP_RIGHT:
            l_newAnchorOffset = {l_elementRelativeSpace.position.x + l_elementRelativeSpace.size.x, l_elementRelativeSpace.position.y};
            break;
        case UIAnchor::LEFT:
            l_newAnchorOffset = {l_elementRelativeSpace.position.x, l_elementRelativeSpace.position.y + l_elementRelativeSpace.size.y / 2.f};
            break;
        case UIAnchor::CENTER:
            l_newAnchorOffset = {l_elementRelativeSpace.position.x + l_elementRelativeSpace.size.x / 2.f, l_elementRelativeSpace.position.y + l_elementRelativeSpace.size.y / 2.f};
            break;
        case UIAnchor::RIGHT:
            l_newAnchorOffset = {l_elementRelativeSpace.position.x + l_elementRelativeSpace.size.x, l_elementRelativeSpace.position.y + l_elementRelativeSpace.size.y / 2.f};
            break;
        case UIAnchor::BOTTOM_LEFT:
            l_newAnchorOffset = {l_elementRelativeSpace.position.x, l_elementRelativeSpace.position.y + l_elementRelativeSpace.size.y};
            break;
        case UIAnchor::BOTTOM:
            l_newAnchorOffset = {l_elementRelativeSpace.position.x + l_elementRelativeSpace.size.x / 2.f, l_elementRelativeSpace.position.y + l_elementRelativeSpace.size.y};
            break;
        case UIAnchor::BOTTOM_RIGHT:
            l_newAnchorOffset = {l_elementRelativeSpace.position.x + l_elementRelativeSpace.size.x, l_elementRelativeSpace.position.y + l_elementRelativeSpace.size.y};
            break;
        default:
            l_newAnchorOffset = l_elementRelativeSpace.position;
            break;
    }

    m_anchorOffset = l_newAnchorOffset;
}