#include "ui_animation.hpp"
#include "../components/ui_component.hpp"
#include "../ui_element.hpp"
#include "ui_animation_data.hpp"
#include "../../core/game.hpp"
#include <SFML/Graphics/Rect.hpp>

UIAnimation::UIAnimation(UIComponent* component, UIAnimationData* data) : component(component), data(data), running(false), elementAnimation(false) {}

UIAnimation::UIAnimation(UIElement* element, UIAnimationData* data) : element(element), data(data), running(false), elementAnimation(true) {}

void UIAnimation::restart(bool startAnimation)
{
    UIPosition* targetPosition;

    if (elementAnimation)
    {
        if (!element)
        {
            std::cout << "ERROR in UIAnimation::restart(). elementAnimation is true, but there is no element pointer. element = " << element << ".\n";
            assert(false);
        }

        targetPosition = &element->position;
    }
    else
    {
        if (!component)
        {
            std::cout << "ERROR in UIAnimation::restart(). elementAnimation is false (meaning component should be used), but there is no component pointer. component = " << component << ".\n";
            assert(false);
        }

        targetPosition = &component->position;
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

    sf::Vector2f endOriginOffset;
    sf::Vector2f endAnchorOffset;
    if (elementAnimation)
    {
        sf::FloatRect bb = element->getGlobalBounds();

        endOriginOffset = UIPosition::getOriginOffset(element->position, bb.size);
        endAnchorOffset = UIPosition::getAnchorOffset(element);

        sf::Vector2f effPos = element->effectivePosition;
        endOriginOffset += (effPos - bb.position);
    }
    else
    {
        endOriginOffset = UIPosition::getOriginOffset(component->position, component->getLocalBounds().size);
        endAnchorOffset = UIPosition::getAnchorOffset(component->position, component->myElement->getLocalBoundsUpToComponent(component->sortIndex));
    }

    endPosition = data->endPosition + endOriginOffset;
    if (data->relativePos) endPosition += targetPosition->position;



    targetPosition->origin = startOrigin;
    targetPosition->anchor = startAnchor;

    sf::Vector2f startOriginOffset;
    sf::Vector2f startAnchorOffset;
    if (elementAnimation)
    {
        // sf::FloatRect bb = element->getGlobalBounds();
    
        // startOriginOffset = UIPosition::getOriginOffset(element->position, bb.size);
        startAnchorOffset = element->anchorOffset;

        // sf::Vector2f effPos = element->effectivePosition;
        // startOriginOffset += (effPos - bb.position);
    }
    else
    {
        startOriginOffset = component->originOffset;
        startAnchorOffset = component->anchorOffset;
    }
    
    if (elementAnimation)
    {
        startPosition = data->startPosition;
        sf::Vector2f positionAfterDistance = data->endPosition + (endAnchorOffset - startAnchorOffset) + endOriginOffset;
        distance = positionAfterDistance - startPosition;
        if (data->relativePos)
        {
            startPosition += targetPosition->position;
            distance += targetPosition->position;
        }
        
        element->updateVisuals();
    }
    else
    {
        sf::Vector2f startTotalOffset = startOriginOffset + startAnchorOffset;
        sf::Vector2f endTotalOffset = endOriginOffset + endAnchorOffset;
        
        startPosition = data->startPosition;
        sf::Vector2f positionAfterDistance = data->endPosition + (endTotalOffset - startTotalOffset);
        distance = positionAfterDistance - startPosition;
        if (data->relativePos)
        {
            startPosition += targetPosition->position;
            distance += targetPosition->position;
        }

        component->updateVisuals();
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
            // std::cout << "curr: " << element->position.position.x << ", " << element->position.position.y << '\n';
            // std::cout << "new: " << currentPosition.x << ", " << currentPosition.y << '\n';

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