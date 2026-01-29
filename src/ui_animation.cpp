#include "ui_animation.hpp"
#include "ui_layer.hpp"

UIAnimation::UIAnimation() {}

UIAnimation::UIAnimation(sf::Vector2f startCenter, sf::Vector2f endCenter, int endPosSet)
{
    init(startCenter, endCenter, endPosSet);
}

void UIAnimation::init(sf::Vector2f startCenter, sf::Vector2f endCenter, int endPosSet)
{
    currentCenter = startCenter;

    this->startCenter = startCenter;

    this->endCenter = endCenter;

    this->endPosSet = endPosSet;
}

sf::Vector2f UIAnimation::step()
{
    float xDiff = endCenter.x - currentCenter.x;
    float yDiff = endCenter.y - currentCenter.y;

    currentCenter.x += xDiff / 20.f;
    currentCenter.y += yDiff / 20.f;

    return currentCenter;
}

bool UIAnimation::isDone()
{
    if (getDistance(currentCenter, endCenter) < 0.1f)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void UIAnimation::finalize(UILayer* uiLayer, UIElement* element)
{
    sf::Vector2f convertedPosition;

    float HALFLENGTH = (element->getSize().x / 2.f);
    float HALFWIDTH = (element->getSize().y / 2.f);

    // x
    switch(endPosSet)
    {
        case 0:
        case 2:
        case 6:
            (element->getParent()) ? convertedPosition.x = endCenter.x - HALFLENGTH - element->getParent()->left() : convertedPosition.x = endCenter.x - HALFLENGTH;
            break;
        case 1:
        case 3:
        case 7:    
            (element->getParent()) ? convertedPosition.x = endCenter.x + HALFLENGTH - element->getParent()->right() : convertedPosition.x = endCenter.x + HALFLENGTH - uiLayer->getViewSize().x;
            break;
        default:    
            (element->getParent()) ? convertedPosition.x = endCenter.x - element->getParent()->getScreenCenter().x : convertedPosition.x = endCenter.x;
            break;
    }
  
    // y
    switch(endPosSet)
    {
        case 0:
        case 1:
        case 5:
            (element->getParent()) ? convertedPosition.y = endCenter.y - HALFWIDTH - element->getParent()->top() : convertedPosition.y = endCenter.y - HALFWIDTH;
            break;
        case 2:
        case 3:
        case 8:    
            (element->getParent()) ? convertedPosition.y = endCenter.y + HALFWIDTH - element->getParent()->bottom() : convertedPosition.y = endCenter.y + HALFWIDTH - uiLayer->getViewSize().y;
            break;
        default:
            (element->getParent()) ? convertedPosition.y = endCenter.y - element->getParent()->getScreenCenter().y : convertedPosition.y = endCenter.y;
            break;
    }

    element->setPosSet(endPosSet);
    element->setPosition(convertedPosition);
}