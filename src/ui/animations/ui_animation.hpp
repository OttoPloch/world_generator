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
    sf::Vector2f startPosition;
    sf::Vector2f distance;
    sf::Vector2f endPosition;
    UIOrigin startOrigin;
    UIOrigin endOrigin;
    UIAnchor startAnchor;
    UIAnchor endAnchor;

    // values that change
    float timeProgress;
    bool running;
    sf::Vector2f currentPosition;

    UIComponent* component;
    UIElement* element;
    bool elementAnimation;
};