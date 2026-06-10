#pragma once

#include "../../core/common.hpp"

class UIComponent;
struct UIAnimationData;

struct UIAnimation
{
    UIAnimation();

    UIAnimation(UIComponent* component, UIAnimationData* data);
    
    UIAnimation(UIElement* element, UIAnimationData* data);

    void restart(bool startAnimation);

    void update(float dt);
private:
    void updateChanges();

    void complete();

    // values stay the same
    UIAnimationData* data;

    // values that change
    float timeProgress;
    bool running;
    sf::Vector2f currentPosition;

    UIComponent* component;
    UIElement* element;
    bool elementAnimation;
};