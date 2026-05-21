#include "ui_position.hpp"
#include "ui_element.hpp"
#include "../core/game.hpp"

UIPosition::UIPosition(sf::Vector2f position, UIOrigin origin, UIAnchor anchor, bool worldPosition) : position(position), origin(origin), anchor(anchor), worldPosition(worldPosition) {}

sf::Vector2f UIPosition::getOriginOffset(UIOrigin origin, sf::Vector2f size)
{
    sf::Vector2f offset;

    switch(origin)
    {
        case UIOrigin::TOP_LEFT:
        {
            offset = {0, 0};
            break;
        }
        case UIOrigin::TOP_MIDDLE:
        {
            offset = {-size.x / 2, 0};
            break;
        }
        case UIOrigin::TOP_RIGHT:
        {
            offset = {-size.x, 0};
            break;
        }
        case UIOrigin::LEFT_MIDDLE:
        {
            offset = {0, -size.y / 2};
            break;
        }
        case UIOrigin::CENTER:
        {
            offset = {-size.x / 2, -size.y / 2};
            break;
        }
        case UIOrigin::RIGHT_MIDDLE:
        {
            offset = {-size.x, -size.y / 2};
            break;
        }
        case UIOrigin::BOTTOM_LEFT:
        {
            offset = {0, -size.y};
            break;
        }
        case UIOrigin::BOTTOM_MIDDLE:
        {
            offset = {-size.x / 2, -size.y};
            break;
        }
        case UIOrigin::BOTTOM_RIGHT:
        {
            offset = -size;
            break;
        }
        default:
        {
            offset = {0, 0};
            break;
        }
    }

    return offset;
}

sf::Vector2f UIPosition::getAnchorOffset(UIAnchor anchor, UIElement* element)
{
    sf::Vector2f offset;

    // the space the element is put in. It can go beyond this,
    // but this determines where the anchor can go.
    sf::Vector2f relativeSpace(0, 0);

    if (element->parent)
    {
        relativeSpace = element->parent->getGlobalBounds().size;
    }
    else
    {
        relativeSpace = element->game->getScene()->getUILayer()->getUIView().getSize();
    }

    switch (anchor)
    {
        case UIAnchor::TOP_LEFT:
        {
            offset = {0, 0};
            break;
        }
        case UIAnchor::TOP_MIDDLE:
        {
            offset = {relativeSpace.x / 2, 0};
            break;
        }
        case UIAnchor::TOP_RIGHT:
        {
            offset = {relativeSpace.x, 0};
            break;
        }
        case UIAnchor::LEFT_MIDDLE:
        {
            offset = {0, relativeSpace.y / 2};
            break;
        }
        case UIAnchor::CENTER:
        {
            offset = {relativeSpace.x / 2, relativeSpace.y / 2};
            break;
        }
        case UIAnchor::RIGHT_MIDDLE:
        {
            offset = {relativeSpace.x, relativeSpace.y / 2};
            break;
        }
        case UIAnchor::BOTTOM_LEFT:
        {
            offset = {0, relativeSpace.y};
            break;
        }
        case UIAnchor::BOTTOM_MIDDLE:
        {
            offset = {relativeSpace.x / 2, relativeSpace.y};
            break;
        }
        case UIAnchor::BOTTOM_RIGHT:
        {
            offset = relativeSpace;
            break;
        }
        default:
        {
            offset = {0, 0};
            break;
        }
    }

    return offset;
}