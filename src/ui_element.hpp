#pragma once

#include "common.hpp"

class UILayer;

class UIElement
{
public:
    UIElement();

    // posSet determines if the given position is the top left, top right, bottom left, bottom right or center of the element
    UIElement(UILayer* uiLayer, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, int parentIndex = -1);

    // posSet determines if the given position is the top left, top right, bottom left, bottom right or center of the element
    void init(UILayer* uiLayer, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, int parentIndex = -1);

    // returns the center of this element on the
    // parent element, if this element has one.
    sf::Vector2f getScreenCenter();

    // gets this elements center, if this element has a
    // parent then the position on screen will be different.
    sf::Vector2f getLocalCenter();

    float left();

    float right();
    
    float top();

    float bottom();

    virtual void resize(sf::Vector2f newSize, int posSet);

    virtual void draw();

    sf::Vector2f getSize();
protected:
    UIElement* getParent();

    UILayer* uiLayer;

    unsigned int posSet;

    sf::Vector2f position;

    sf::Vector2f size;

    int parentIndex;
};