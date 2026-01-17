#include "ui_element.hpp"
#include "ui_layer.hpp"

UIElement::UIElement() {}

UIElement::UIElement(Game* game, UILayer* uiLayer, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, int parentID)
{
    init(game, uiLayer, ID, posSet, position, size, parentID);
}

void UIElement::init(Game* game, UILayer* uiLayer, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, int parentID)
{
    this->game = game;

    this->uiLayer = uiLayer;

    this->ID = ID;

    this->posSet = posSet;

    this->position = position;

    this->size = size;

    this->parentID = parentID;
}

void UIElement::setParent(int parentID)
{
    this->parentID = parentID;
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
        case 4:
            center = position;
            break;
        case 5:
            center = {position.x, position.y + size.y / 2.f};
            break;
        case 6:
            center = {position.x + size.x / 2.f, position.y};
            break;
        case 7:
            center = {position.x - size.x / 2.f, position.y};
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
        case 6:
            (getParent()) ? left = getParent()->left() + position.x : left = position.x;
            break;
        case 1:
        case 3:
        case 7:
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
        case 5:
            (getParent()) ? top = getParent()->top() + position.y : top = position.y;
            break;
        case 2:
        case 3:
        case 8:
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

UIElement* UIElement::getParent() { return uiLayer->getElement(parentID); }

int UIElement::getID() { return ID; }

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