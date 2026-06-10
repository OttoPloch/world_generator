#include "ui_animation.hpp"
#include "../components/ui_component.hpp"
#include "../ui_element.hpp"
#include "ui_animation_data.hpp"

UIAnimation::UIAnimation() {}

UIAnimation::UIAnimation(UIComponent* component, UIAnimationData* data) : component(component), data(data), running(false), elementAnimation(false) {}

UIAnimation::UIAnimation(UIElement* element, UIAnimationData* data) : element(element), data(data), running(false), elementAnimation(true) {}

void UIAnimation::restart(bool startAnimation)
{
    timeProgress = 0;
    running = startAnimation;
    currentPosition = data->startPosition;
}

void UIAnimation::update(float dt)
{
    if (running)
    {
        timeProgress += dt;
    
        if (timeProgress >= data->timeToComplete)
        {
            complete();
        }
        else
        {
            currentPosition += (data->endPosition - data->startPosition) * (dt / data->timeToComplete);
    
            updateChanges();
        }
    }
}

void UIAnimation::updateChanges()
{
    if (elementAnimation)
    {
        if (element)
        {
            element->position.position = currentPosition;
            element->updateVisuals();
        }
    }
    else
    {
        if (component)
        {
            component->position.position = currentPosition;
            component->updateVisuals();
        }
    }
}

void UIAnimation::complete()
{
    timeProgress = 0.f;
    running = false;
    currentPosition = data->endPosition;

    updateChanges();
}