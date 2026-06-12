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
    if (data->startOrigin) adjustedStartOrigin = *data->startOrigin;
    else adjustedStartOrigin = targetPosition->origin;

    if (data->endOrigin) adjustedEndOrigin = *data->endOrigin;
    else adjustedEndOrigin = targetPosition->origin;

    if (data->startAnchor) adjustedStartAnchor = *data->startAnchor;
    else adjustedStartAnchor = targetPosition->anchor;
    
    if (data->endAnchor) adjustedEndAnchor = *data->endAnchor;
    else adjustedEndAnchor = targetPosition->anchor;



    targetPosition->origin = adjustedEndOrigin;
    targetPosition->anchor = adjustedEndAnchor;

    sf::Vector2f endOriginAnchorOriginOffset;
    sf::Vector2f endOriginAnchorAnchorOffset;
    if (elementAnimation)
    {
        if (element)
        {
            // NOTE: the origin has no effect on elements, only components, which
            // is why the origin offset here is always set to {0, 0}.

            endOriginAnchorOriginOffset = {0, 0};
            endOriginAnchorAnchorOffset = UIPosition::getAnchorOffset(element);
        }
    }
    else
    {
        if (component)
        {
            endOriginAnchorOriginOffset = UIPosition::getOriginOffset(component->position, component->getLocalBounds().size);
            endOriginAnchorAnchorOffset = UIPosition::getAnchorOffset(component->position, component->myElement->getLocalBoundsUpToComponent(component->sortIndex));
        }
    }
    
    endOriginAnchorEndPosition = data->endPosition;
    if (data->relativePos) endOriginAnchorEndPosition += targetPosition->position;

    targetPosition->origin = adjustedStartOrigin;
    targetPosition->anchor = adjustedStartAnchor;

    sf::Vector2f startOriginAnchorOriginOffset;
    sf::Vector2f startOriginAnchorAnchorOffset;
    if (elementAnimation)
    {
        if (element)
        {
            // NOTE: the origin has no effect on elements, only components, which
            // is why the origin offset here is always set to {0, 0}.

            startOriginAnchorOriginOffset = {0, 0};
            startOriginAnchorAnchorOffset = UIPosition::getAnchorOffset(element);
        }
    }
    else
    {
        if (component)
        {
            startOriginAnchorOriginOffset = UIPosition::getOriginOffset(component->position, component->getLocalBounds().size);
            startOriginAnchorAnchorOffset = UIPosition::getAnchorOffset(component->position, component->myElement->getLocalBoundsUpToComponent(component->sortIndex));
        }
    }

    sf::Vector2f startOriginAnchorTotalOffset = startOriginAnchorOriginOffset + startOriginAnchorAnchorOffset;
    sf::Vector2f endOriginAnchorTotalOffset = endOriginAnchorOriginOffset + endOriginAnchorAnchorOffset;
    
    startOriginAnchorStartPosition = data->startPosition; // FINISH HERE
    startOriginAnchorEndPosition = data->endPosition + (endOriginAnchorTotalOffset - startOriginAnchorTotalOffset);
    if (data->relativePos)
    {
        startOriginAnchorStartPosition += targetPosition->position;
        startOriginAnchorEndPosition += targetPosition->position;
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
    currentPosition = startOriginAnchorStartPosition;
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
            currentPosition += (startOriginAnchorEndPosition - startOriginAnchorStartPosition) * (dt / data->timeToComplete);
    
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
    currentPosition = endOriginAnchorEndPosition;

    if (elementAnimation)
    {
        if (element)
        {
            element->position.origin = adjustedEndOrigin;
            element->position.anchor = adjustedEndAnchor;
        }
    }
    else
    {
        if (component)
        {
            component->position.origin = adjustedEndOrigin;
            component->position.anchor = adjustedEndAnchor;
        }
    }

    updateChanges();
}