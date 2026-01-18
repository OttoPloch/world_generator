#include "ui_element.hpp"
#include "game.hpp"
#include "ui_layer.hpp"
#include "ui_background.hpp"
#include "ui_text.hpp"
#include "ui_button.hpp"

UIElement::UIElement() {}

UIElement::UIElement(Game* game, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, std::string parentName)
{
    init(game, name, ID, posSet, position, size, parentName);
}

void UIElement::init(Game* game, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, std::string parentName)
{
    this->game = game;

    this->uiLayer = game->getScene()->getUILayer();

    this->name = name;

    this->ID = ID;

    this->posSet = posSet;

    this->position = position;

    this->size = size;

    setParent(parentName);
}

void UIElement::setParent(std::string parentName)
{
    (parentName != "") ? parentID = uiLayer->getElement(parentName)->getID() : parentID = -1;
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

std::string UIElement::getName() { return name; }

int UIElement::getID() { return ID; }

void UIElement::updateSize()
{
    resize(size);
}

UIBackground* UIElement::getAsBackground() { return nullptr; }

UIText* UIElement::getAsText() { return nullptr; }

UIButton* UIElement::getAsButton() { return nullptr; }

void UIElement::resize(sf::Vector2f newSize, int posSet)
{
    size = newSize;
    
    if (posSet != -1) this->posSet = posSet;
}

void UIElement::tick() {}

void UIElement::draw() {}