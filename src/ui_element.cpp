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
            (getParent()) ? left = getParent()->right() + position.x - size.x : left = uiLayer->getScreenSize().x + position.x - size.x;
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
            (getParent()) ? top = getParent()->bottom() + position.y - size.y : top = uiLayer->getScreenSize().y + position.y - size.y;
            break;
        default:
            (getParent()) ? top = getParent()->getScreenCenter().y + position.y - size.y / 2.f : top = (uiLayer->getScreenSize().y / 2.f) + position.y - size.y / 2.f;
            break;
    }

    return top;
}

float UIElement::bottom()
{
    return top() + size.y;
}

sf::Vector2f UIElement::getSize() { return size; }

UIElement* UIElement::getParent() { return uiLayer->getElement(parentIndex); }

int UIElement::getParentIndex() { return parentIndex; }

void UIElement::updateSize()
{
    resize(size);
}

void UIElement::resize(sf::Vector2f newSize, int posSet)
{
    size = newSize;
    
    if (posSet != -1) this->posSet = posSet;
}

void UIElement::draw() {}