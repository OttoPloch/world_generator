#pragma once

#include "common.hpp"

class UILayer;

class UIElement
{
public:
    UIElement();

    // posSet determines if the given position is the top left, top right, bottom left, bottom right or center of the element
    UIElement(Game* game, UILayer* uiLayer, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, int parentID = -1);

    // posSet determines if the given position is the top left, top right, bottom left, bottom right or center of the element
    void init(Game* game, UILayer* uiLayer, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, int parentID = -1);

    void setParent(int parentID);

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

    sf::Vector2f getSize();

    int getID();

    // updates everything without changing size.
    void updateSize();

    // If posSet is -1, then the element's posSet will not change.
    virtual void resize(sf::Vector2f newSize, int posSet = -1);

    virtual void draw();
protected:
    UIElement* getParent();

    Game* game;

    UILayer* uiLayer;

    int ID;
    
    unsigned int posSet;

    sf::Vector2f position;

    sf::Vector2f size;

    int parentID;
};