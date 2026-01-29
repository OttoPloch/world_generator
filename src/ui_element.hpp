#pragma once

#include "common.hpp"

class UILayer;
class UIBackground;
class UIText;
class UIButton;
class UIAnimation;

class UIElement
{
public:
    UIElement();

    // posSets:
    // (9 sets center with 0's anchor [none])
    //  _____________
    // |0     5     1|
    // |             |
    // |6     4     7|
    // |             |
    // |2     8     3|
    //  -------------

    // posSet determines the anchor/origin of the element
    UIElement(Game* game, UILayer* uiLayer, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, UIElement* parent = nullptr);
    
    // posSet determines the anchor/origin of the element
    void baseInit(Game* game, UILayer* uiLayer, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, UIElement* parent = nullptr);

    void setParent(UIElement* parent);

    UIElement* getParent();

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

    void setPosSet(int newPosSet);

    int getPosSet();

    sf::Vector2f getPosition();

    void setPosition(sf::Vector2f position);

    void movePosition(sf::Vector2f amount);

    // endPosSet of -1 means no change
    void setAnimation(sf::Vector2f startPosition, sf::Vector2f endPosition, int startPosSet = -1, int endPosSet = -1, bool useOwnStartPos = false, bool useOwnEndPos = false);

    bool hasAnimation();

    // if the element has an animation/is moving to keep up with an element that does.
    // this is used so that all the elements attached to one with an animation stay together.
    bool isMoving();

    void baseUpdate(float dt);

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

    std::unique_ptr<UIAnimation> animation;
};