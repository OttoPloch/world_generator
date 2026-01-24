#pragma once

#include "common.hpp"

class UILayer;
class UIBackground;
class UIText;
class UIButton;

class UIElement
{
public:
    UIElement();

    // posSet determines if the given position is the top left, top right, bottom left, bottom right or center of the element
    UIElement(Game* game, UILayer* uiLayer, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, UIElement* parent = nullptr);

    // posSet determines if the given position is the top left, top right, bottom left, bottom right or center of the element
    void baseInit(Game* game, UILayer* uiLayer, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, UIElement* parent = nullptr);

    void setParent(UIElement* parent);

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

    std::string getName();

    int getID();

    // updates everything without changing size.
    void updateSize();

    void setPosition(sf::Vector2f position);

    void movePosition(sf::Vector2f amount);

    // not sure if this is a good way to do this
    virtual UIBackground* getAsBackground();
    
    // same as above
    virtual UIText* getAsText();
    
    // same as above
    virtual UIButton* getAsButton();

    // If posSet is -1, then the element's posSet will not change.
    virtual void resize(sf::Vector2f newSize, int posSet = -1);

    virtual void update();

    virtual void draw();
protected:
    Game* game;

    UILayer* uiLayer;

    std::string name;

    int ID;
    
    unsigned int posSet;

    sf::Vector2f position;

    sf::Vector2f size;

    UIElement* parent;
};