#pragma once

#include "../../core/common.hpp"
#include "../ui_position.hpp"

class UIComponent;
struct UIAnimationData;

struct UIAnimation
{
    UIAnimation(UIComponent* component, UIAnimationData* data);
    
    UIAnimation(UIElement* element, UIAnimationData* data);

    void restart(bool startAnimation);

    void update(float dt);
private:
    void updateChanges();

    void complete();

    // values stay the same
    UIAnimationData* data;
    sf::Vector2f startOriginAnchorStartPosition;
    sf::Vector2f startOriginAnchorEndPosition;
    sf::Vector2f endOriginAnchorEndPosition;
    UIOrigin adjustedStartOrigin;
    UIOrigin adjustedEndOrigin;
    UIAnchor adjustedStartAnchor;
    UIAnchor adjustedEndAnchor;

    // values that change
    float timeProgress;
    bool running;
    sf::Vector2f currentPosition;

    UIComponent* component;
    UIElement* element;
    bool elementAnimation;
};