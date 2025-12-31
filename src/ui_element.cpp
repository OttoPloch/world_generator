#include "ui_element.hpp"
#include "ui_layer.hpp"

UIElement::UIElement() {}

UIElement::UIElement(UILayer* uiLayer, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, int parentIndex)
{
    init(uiLayer, posSet, position, size, parentIndex);
}

void UIElement::init(UILayer* uiLayer, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, int parentIndex)
{
    this->uiLayer = uiLayer;

    this->posSet = posSet;

    this->position = position;

    this->size = size;

    this->parentIndex = parentIndex;
}

sf::Vector2f UIElement::getScreenCenter()
{
    return {left() + size.x / 2.f, top() + size.y / 2.f};
}

sf::Vector2f UIElement::getLocalCenter()
{
    sf::Vector2f center;

    switch(posSet)
    {
        case 0:
            center = {position.x + size.x / 2.f, position.y + size.y / 2.f};
            break;
        case 1:
            center = {position.x - size.x / 2.f, position.y + size.y / 2.f};
            break;
        case 2:
            center = {position.x + size.x / 2.f, position.y - size.y / 2.f};
            break;
        case 3:
            center = {position.x - size.x / 2.f, position.y - size.y / 2.f};
            break;
        default:
            center = position;
            break;
    }

    return center;
}

float UIElement::left()
{
    float left;

    switch(posSet)
    {
        case 0:
        case 2:
            (getParent()) ? left = getParent()->left() + position.x : left = position.x;
            break;
        case 1:
        case 3:
            (getParent()) ? left = getParent()->right() + position.x - size.x : left = position.x - size.x;
            break;
        default:
            (getParent()) ? left = getParent()->getScreenCenter().x + position.x - size.x / 2.f : left = (uiLayer->getScreenSize().x / 2.f) + position.x - size.x / 2.f;
            break;
    }

    return left;
}

float UIElement::right()
{
    return left() + size.x;

    // float right;

    // switch(posSet)
    // {
    //     case 0:
    //     case 2:
    //         (getParent()) ? right = getParent()->left() + position.x + size.x : right = position.x + size.x;
    //         break;
    //     case 1:
    //     case 3:
    //         (getParent()) ? right = getParent()->right() + position.x - size.x : right = position.x;
    //         break;
    //     default:
    //         right = getScreenPosition().x + size.x / 2.f;
    //         break;
    // }

    // return right;
}

float UIElement::top()
{
    float top;

    switch(posSet)
    {
        case 0:
        case 1:
            (getParent()) ? top = getParent()->top() + position.y : top = position.y;
            break;
        case 2:
        case 3:
            (getParent()) ? top = getParent()->bottom() + position.y - size.y : top = position.y - size.y;
            break;
        default:
            (getParent()) ? top = getParent()->getScreenCenter().y + position.y - size.y / 2.f: top = (uiLayer->getScreenSize().y / 2.f) + position.y - size.y / 2.f;
            break;
    }

    return top;
}

float UIElement::bottom()
{
    return top() + size.y;

    // float bottom;

    // switch(posSet)
    // {
    //     case 0:
    //     case 1:
    //         (getParent()) ? bottom = getParent()->top() + position.y + size.y : bottom = position.y + size.y;
    //         break;
    //     case 2:
    //     case 3:
    //         (getParent()) ? bottom = getParent()->bottom() + position.y : bottom = position.y;
    //         break;
    //     default:
    //         bottom = getScreenPosition().y + size.y / 2.f;
    //         break;
    // }

    // return bottom;
}

sf::Vector2f UIElement::getSize() { return size; }

UIElement* UIElement::getParent() { return uiLayer->getElement(parentIndex); }