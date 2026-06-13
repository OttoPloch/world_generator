#include "ui_animation.hpp"
#include "../components/ui_component.hpp"
#include "../ui_element.hpp"
#include "ui_animation_data.hpp"
#include "../../core/game.hpp"

UIAnimation::UIAnimation(UIComponent* component, UIAnimationData* data) : component(component), data(data), running(false), elementAnimation(false) {}

UIAnimation::UIAnimation(UIElement* element, UIAnimationData* data) : element(element), data(data), running(false), elementAnimation(true) {}

void UIAnimation::restart(bool startAnimation)
{
    UIPosition* targetPosition;

    if (elementAnimation)
    {
        if (element) targetPosition = &element->position;
    }
    else
    {
        if (component) targetPosition = &component->position;
    }



    // Setting the adjusted origins and anchors. This needs to
    // happen because UIAnimationData stores these as smart pointers,
    // so if they do not exist there is supposed to be no change in
    // origin or anchor.
    if (data->startOrigin) startOrigin = *data->startOrigin;
    else startOrigin = targetPosition->origin;

    if (data->endOrigin) endOrigin = *data->endOrigin;
    else endOrigin = targetPosition->origin;

    if (data->startAnchor) startAnchor = *data->startAnchor;
    else startAnchor = targetPosition->anchor;
    
    if (data->endAnchor) endAnchor = *data->endAnchor;
    else endAnchor = targetPosition->anchor;



    targetPosition->origin = endOrigin;
    targetPosition->anchor = endAnchor;

    sf::Vector2f finalOriginOffset;
    sf::Vector2f finalAnchorOffset;
    if (elementAnimation)
    {
        if (element)
        {
            finalOriginOffset = UIPosition::getOriginOffset(element->position, element->getGlobalBounds().size);
            finalAnchorOffset = UIPosition::getAnchorOffset(element);
        }
    }
    else
    {
        if (component)
        {
            finalOriginOffset = UIPosition::getOriginOffset(component->position, component->getLocalBounds().size);
            finalAnchorOffset = UIPosition::getAnchorOffset(component->position, component->myElement->getLocalBoundsUpToComponent(component->sortIndex));
        }
    }
    
    endPosition = data->endPosition;
    if (data->relativePos) endPosition += targetPosition->position;

    targetPosition->origin = startOrigin;
    targetPosition->anchor = startAnchor;

    sf::Vector2f startOriginOffset;
    sf::Vector2f startAnchorOffset;
    if (elementAnimation)
    {
        if (element)
        {
            startOriginOffset = UIPosition::getOriginOffset(element->position, element->getGlobalBounds().size);
            startAnchorOffset = UIPosition::getAnchorOffset(element);
        }
    }
    else
    {
        if (component)
        {
            startOriginOffset = UIPosition::getOriginOffset(component->position, component->getLocalBounds().size);
            startAnchorOffset = UIPosition::getAnchorOffset(component->position, component->myElement->getLocalBoundsUpToComponent(component->sortIndex));
        }
    }

    sf::Vector2f startTotalOffset = startOriginOffset + startAnchorOffset;
    sf::Vector2f endTotalOffset = finalOriginOffset + finalAnchorOffset;
    
    startPosition = data->startPosition;
    sf::Vector2f positionAfterDistance = data->endPosition + (endTotalOffset - startTotalOffset);
    distance = positionAfterDistance - startPosition;
    if (data->relativePos)
    {
        startPosition += targetPosition->position;
        distance += targetPosition->position;
    }

    if (elementAnimation)
    {
        if (element) element->updateVisuals();
    }
    else
    {
        if (component) component->updateVisuals();
    }

    timeProgress = 0;
    running = startAnimation;
    currentPosition = startPosition;
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
            currentPosition += distance * (dt / data->timeToComplete);
    
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
            // element->position.position = currentPosition;

            // since elements do not use origins in the same way as components, 
            // this is an attempt to counteract that.
            element->position.position = element->originOffset + currentPosition;
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
    currentPosition = endPosition;

    if (elementAnimation)
    {
        if (element)
        {
            element->position.origin = endOrigin;
            element->position.anchor = endAnchor;
        }
    }
    else
    {
        if (component)
        {
            component->position.origin = endOrigin;
            component->position.anchor = endAnchor;
        }
    }

    updateChanges();
}