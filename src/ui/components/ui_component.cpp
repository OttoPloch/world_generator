#include "ui_component.hpp"
#include "../ui_element.hpp"
#include "../../core/game.hpp"
#include <SFML/Window/Mouse.hpp>

UIComponent::UIComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex) : game(game), myElement(myElement), position(position), identifier(identifier), sortIndex(sortIndex), pressed(false), pressedLastFrame(false), canPress(false), selected(false), selectedLastFrame(false)
{
    uiState = UIState::IDLE;

    statesToItemName = {
        {UIState::IDLE, "idle"},
        {UIState::HOVER, "hover"},
        {UIState::PRESS, "press"}
    };
}

sf::FloatRect UIComponent::getGlobalBounds()
{
    sf::FloatRect lb = getLocalBounds();

    return {myElement->effectivePosition + lb.position, lb.size};
}

sf::FloatRect UIComponent::getLocalBounds()
{
    return {position.position + originOffset + anchorOffset, {0, 0}};
}

void UIComponent::onIdle() {}

void UIComponent::onHover() {}

void UIComponent::onPress() {}

bool UIComponent::isSelected()
{  
    if (game->getInput()->cursor->isUIModeActive())
    {
        return (game->getInput()->cursor->getSelectedComponent() == this);
    }
    else
    {
        if (!myElement->isComponentOnTopAtPoint(this, game->getInput()->cursor->getGameCursorPosition())) return false;

        sf::FloatRect gb = getGlobalBounds();

        return mouseRectCollide(game, gb.position, gb.size, false);
    }
}

bool UIComponent::isPressed()
{
    return attemptedPress() && canPress;
}

bool UIComponent::justPressed()
{
    return pressed && !pressedLastFrame;
}

void UIComponent::setAnimation(UIAnimationData* data, bool startAnimation)
{
    animation = std::make_unique<UIAnimation>(this, data);
    animation->restart(startAnimation);
}

void UIComponent::resize(sf::Vector2f newSize) {}

void UIComponent::updateVisuals()
{
    originOffset = {0, 0};
    anchorOffset = {0, 0};
}

void UIComponent::updateState()
{
    selectedLastFrame = selected;
    selected = isSelected();

    // Somewhat convoluted method of not allowing the component to be pressed the same frame it was selected.
    // This is designed to prevent misclicks by not letting the cursor be clicked and move onto the component and register as a press.
    // Because of this, there must be a moment where the component is selected but not pressed before it can be registered as pressed.
    if (justSelected())
    {
        canPress = !attemptedPress();
    }
    else
    {
        if (isSelected())
        {
            if (!attemptedPress())
            {
                canPress = true;
            }
        }
        else
        {
            canPress = false;
        }
    }

    // for knowing if just pressed
    pressedLastFrame = pressed;
    pressed = isPressed();

    // for changing textures, tracks new button state
    UIState nextState;
    if (pressed) nextState = UIState::PRESS;
    else if (isSelected()) nextState = UIState::HOVER;
    else nextState = UIState::IDLE;

    // if state changed, change visuals
    if (nextState != uiState)
    { 
        std::cout << "COMPONENT WITH IDENTIFIER '" << identifier << "' CHANGED STATE FROM " << statesToItemName[uiState] << " TO " << statesToItemName[nextState] << "!\n" ;
        
        uiState = nextState;

        switch (uiState)
        {
            case UIState::IDLE:
                onIdle();
                break;
            case UIState::HOVER:
                onHover();
                break;
            case UIState::PRESS:
                onPress();
                break;
            default:
                break;
        }

        updateVisuals();
    }
}

void UIComponent::update(float dt)
{
    if (animation) animation->update(dt);

    updateState();
}

void UIComponent::draw(bool debug) {}


bool UIComponent::attemptedPress()
{
    return isSelected() && game->getInput()->isControlPressed("UI PRESS");
}

bool UIComponent::justSelected()
{
    return selected && !selectedLastFrame;
}
