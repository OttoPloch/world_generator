#include "ui_element.hpp"
#include "../core/game.hpp"
#include "components/ui_component.hpp"
#include "ui_position.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <utility>
#include <algorithm>

UIElement::UIElement(Game* game, std::string name, UIPosition position, int z, UIElement* parent) : game(game), name(name), position(position), z(z), visible(true), parent(parent)
{
    updateVisuals();
}

sf::FloatRect UIElement::getGlobalBounds()
{
    // relative coordinates
    float left = 0, top = 0, right = 0, bottom = 0;
    
    calculateEffectivePosition();

    for (auto& c : components)
    {
        sf::FloatRect cBox = c->getLocalBounds();

        left = std::min(left, cBox.position.x);
        top = std::min(top, cBox.position.y);
        right = std::max(right, cBox.position.x + cBox.size.x);
        bottom = std::max(bottom, cBox.position.y + cBox.size.y);
    }

    // converts to global coordinates
    return {effectivePosition + sf::Vector2f(left, top), {right - left, bottom - top}};
}

std::vector<sf::FloatRect> UIElement::getAllComponentBounds()
{
    std::vector<sf::FloatRect> bounds;

    for (auto& c : components)
    {
        bounds.emplace_back(c->getGlobalBounds());
    }

    return bounds;
}

bool UIElement::isComponentOnTopAtPoint(UIComponent* component, sf::Vector2f point)
{
    if (!pointRectCollide(point, component->getGlobalBounds())) return false;

    std::vector<UIComponent*> collidedComponents;

    for (auto& c : components)
    {
        if (pointRectCollide(point, c->getGlobalBounds()))
        {
            collidedComponents.emplace_back(c.get());
        }
    }

    // may not be necessary as the components should already be added in order;
    std::sort(collidedComponents.begin(), collidedComponents.end(), [](UIComponent* a, UIComponent* b) {
        return a->sortIndex < b->sortIndex;
    });

    bool hasPassedComponent = false;
    for (auto c : collidedComponents)
    {
        // if true, the given component will be drawn under another and is therefore not on top at the point.
        if (hasPassedComponent) return false;

        if (c == component) hasPassedComponent = true;
    }

    return true;
}

sf::FloatRect UIElement::getLocalBoundsUpToComponent(int sortIndex)
{
    float left = 0, top = 0, right = 0, bottom = 0;

    calculateEffectivePosition();

    for (auto& c : components)
    {
        if (c->sortIndex >= sortIndex) break;

        sf::FloatRect cBox = c->getLocalBounds();

        left = std::min(left, cBox.position.x);
        top = std::min(top, cBox.position.y);
        right = std::max(right, cBox.position.x + cBox.size.x);
        bottom = std::max(bottom, cBox.position.y + cBox.size.y);
    }

    return {{left, top}, {right - left, bottom - top}};
}

void UIElement::setAnimation(UIAnimationData* data, bool startAnimation)
{
    animation = std::make_unique<UIAnimation>(this, data);
    animation->restart(startAnimation);
}

UIComponent* UIElement::getNearestComponent(sf::Vector2f direction, UIComponent* component)
{
    if (direction == sf::Vector2f(0, 0)) return component;
    if (position.worldPosition)
    {
        std::cout << "ERROR in UIElement::getNearestComponent(), element's position is in the world, not designed to use this function with an element in the world.\n";
        return nullptr;
    }

    sf::FloatRect bounds;
    if (component)
    {
        bounds = component->getGlobalBounds();
    }
    else
    {
        bounds.size = {0, 0};

        if (direction.x > 0)
        {
            bounds.position = {-999999, 0};
        }
        else if (direction.x < 0)
        {
            bounds.position = {999999, 0};
        }
        else if (direction.y > 0)
        {
            bounds.position = {0, -999999};
        }
        else if (direction.y < 0)
        {
            bounds.position = {0, 999999};
        }
    }

    sf::Vector2f center = {bounds.position.x + bounds.size.x / 2, bounds.position.y + bounds.size.y / 2};

    UIComponent* nearestComponent = nullptr;
    float nearestDistance = MAXFLOAT;

    for (auto& c : components)
    {
        if (c.get() == component || c->identifier.substr(0, 2) == "__") continue;

        sf::FloatRect cBounds = c->getGlobalBounds();
        sf::Vector2f cCenter = {cBounds.position.x + cBounds.size.x / 2, cBounds.position.y + cBounds.size.y / 2};

        float dist = getDistance(center, cCenter);

        // if true, this component is guarenteed to not be the closest no matter the direction.
        if (dist >= nearestDistance) continue;

        // making sure this component is in the correct direction and preventing movement to a component with no diff in that direction
        sf::Vector2f diff = cCenter - center;
        if (diff.x >= 0 && direction.x < 0) continue;
        if (diff.x <= 0 && direction.x > 0) continue;
        if (diff.y >= 0 && direction.y < 0) continue;
        if (diff.y <= 0 && direction.y > 0) continue;

        nearestComponent = c.get();
        nearestDistance = dist;
    }

    return nearestComponent;
}

void UIElement::updateVisuals()
{
    anchorOffset = UIPosition::getAnchorOffset(this);

    calculateEffectivePosition();

    for (auto& c : components)
    {
        c->updateVisuals();
    }
}

void UIElement::update(float dt)
{
    if (animation) animation->update(dt);

    for (auto& c : components) c->update(dt);
}

void UIElement::draw(bool debug)
{
    if (!visible) return;

    for (auto& c : components) c->draw(debug);
}

sf::Vector2f UIElement::calculateEffectivePosition()
{
    if (parent)
    {
        effectivePosition = parent->getGlobalBounds().position + anchorOffset + position.position;
    }
    else
    {
        effectivePosition = anchorOffset + position.position;
    }

    return effectivePosition;
}

void UIElement::sortComponents()
{
    std::sort(components.begin(), components.end(), [](const std::unique_ptr<UIComponent>& a, const std::unique_ptr<UIComponent>& b){
        return a->sortIndex < b->sortIndex;
    });
}