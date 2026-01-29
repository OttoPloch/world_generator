#include "ui_element.hpp"
#include "game.hpp"
#include "ui_layer.hpp"
#include "ui_background.hpp"
#include "ui_text.hpp"
#include "ui_button.hpp"
#include "ui_animation.hpp"

UIElement::UIElement() {}

UIElement::UIElement(Game* game, UILayer* uiLayer, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, UIElement* parent)
{
    baseInit(game, uiLayer, name, ID, posSet, position, size, parent);
}

void UIElement::baseInit(Game* game, UILayer* uiLayer, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, UIElement* parent)
{
    this->game = game;

    this->uiLayer = uiLayer;

    this->name = name;

    this->ID = ID;

    this->posSet = posSet;

    this->position = position;

    this->size = size;

    this->parent = parent;
}

void UIElement::setParent(UIElement* parent)
{
    this->parent = parent;
}

UIElement* UIElement::getParent()
{
    return parent;
}

sf::Vector2f UIElement::getScreenCenter()
{
    return {left() + size.x / 2.f, top() + size.y / 2.f};
}

sf::Vector2f UIElement::getLocalCenter()
{
    sf::Vector2f center;

    // x
    switch(posSet)
    {
        case 0:
        case 2:
        case 6:
            center.x = position.x + size.x / 2.f;
            break;
        case 1:
        case 3:
        case 7:
            center.x = position.x - size.x / 2.f;
            break;
        default:
            center.x = position.x;
            break;
    }
        
    // y
    switch(posSet)
    {
        case 0:
        case 1:
        case 5:
            center.y = position.y + size.x / 2.f;
            break;
        case 2:
        case 3:
        case 8:
            center.y = position.y - size.x / 2.f;
            break;
        default:
            center.y = position.y;
            break;
    }

    return center;
}

float UIElement::left()
{
    float left = 0.f;
    
    switch(posSet)
    {
        case 0:
        case 2:
        case 6:
            (parent) ? left = parent->left() + position.x : left = position.x;
            break;
        case 1:
        case 3:
        case 7:
            (parent) ? left = parent->right() + position.x - size.x : left = uiLayer->getViewSize().x + position.x - size.x;
            break;
        case 9:
            (parent) ? left = parent->left() + position.x - size.x / 2.f : left = position.x - size.x / 2.f;
            break;
        default:
            (parent) ? left = parent->getScreenCenter().x + position.x - size.x / 2.f : left = (uiLayer->getViewSize().x / 2.f) + position.x - size.x / 2.f;
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
    float top = 0.f;

    switch(posSet)
    {
        case 0:
        case 1:
        case 5:
            (parent) ? top = parent->top() + position.y : top = position.y;
            break;
        case 2:
        case 3:
        case 8:
            (parent) ? top = parent->bottom() + position.y - size.y : top = uiLayer->getViewSize().y + position.y - size.y;
            break;
        case 9:
            (parent) ? top = parent->top() + position.y - size.y / 2.f : top = position.y - size.y / 2.f;
            break;
        default:
            (parent) ? top = parent->getScreenCenter().y + position.y - size.y / 2.f : top = (uiLayer->getViewSize().y / 2.f) + position.y - size.y / 2.f;
            break;
    }

    return top;
}

float UIElement::bottom()
{
    return top() + size.y;
}

sf::Vector2f UIElement::getSize() { return size; }

std::string UIElement::getName() { return name; }

int UIElement::getID() { return ID; }

void UIElement::updateSize()
{
    resize(size);
}

void UIElement::setPosSet(int newPosSet)
{
    posSet = newPosSet;
}

int UIElement::getPosSet()
{
    return posSet;
}

sf::Vector2f UIElement::getPosition() { return position; }

void UIElement::setPosition(sf::Vector2f position)
{
    this->position = position;

    updateSize();
}

void UIElement::movePosition(sf::Vector2f amount)
{
    position.x += amount.x;
    position.y += amount.y;
    
    updateSize();
}

void UIElement::setAnimation(sf::Vector2f startPosition, sf::Vector2f endPosition, int startPosSet, int endPosSet, bool useOwnStartPos, bool useOwnEndPos)
{
    sf::Vector2f start = startPosition;
    sf::Vector2f end = endPosition;
    int set = endPosSet;
    
    if (endPosSet == -1) set = posSet;

    if (useOwnStartPos)
    {
        start = getScreenCenter();
    }
    else
    {
        int startSet = startPosSet;
        if (startPosSet == -1) startSet = posSet;

        start = UIPositionToScreenCenter(uiLayer, this, startPosition, startSet);
    }
    
    if (useOwnEndPos)
    {
        end = getScreenCenter();
    }
    else
    {
        end = UIPositionToScreenCenter(uiLayer, this, endPosition, endPosSet);
    }

    this->animation = std::make_unique<UIAnimation>(start, end, set);

    posSet = 9;
}

bool UIElement::hasAnimation()
{
    return animation.get() != nullptr;
}

bool UIElement::isMoving()
{
    if (parent && (parent->hasAnimation() || parent->isMoving()))
    {
        return true;
    }

    return false;
}

void UIElement::baseUpdate()
{
    if (animation.get() != nullptr)
    {
        setPosition(animation->step());

        if (animation->isDone())
        {
            animation->finalize(uiLayer, this);

            animation.reset();
        }
    }

    if (isMoving())
    {
        updateSize();
    }

    update();
}

UIBackground* UIElement::getAsBackground() { return nullptr; }

UIText* UIElement::getAsText() { return nullptr; }

UIButton* UIElement::getAsButton() { return nullptr; }

void UIElement::resize(sf::Vector2f newSize, int posSet)
{
    size = newSize;
    
    if (posSet != -1) this->posSet = posSet;
}

void UIElement::update() {}

void UIElement::draw() {}