#include "ui_position.hpp"

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