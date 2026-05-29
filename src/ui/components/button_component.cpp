#include "button_component.hpp"
#include "../../graphics/vertex_group.hpp"
#include "../../core/game.hpp"
#include <SFML/Graphics/CoordinateType.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/StencilMode.hpp>

ButtonComponent::ButtonComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, std::map<ButtonState, sf::Texture*> textures, sf::Vector2f size, bool sizeIsScale) : UIComponent(game, myElement, position, identifier, sortIndex), pressed(false), pressedLastFrame(false), canPress(false), selected(false), selectedLastFrame(false)
{
    buttonState = ButtonState::UP;

    setButtonTextures(textures, size, sizeIsScale);
}

sf::FloatRect ButtonComponent::getLocalBounds()
{
    return {position.position + originOffset + anchorOffset, size};
}

void ButtonComponent::updateVisuals()
{
    originOffset = UIPosition::getOriginOffset(position.origin, size);
    anchorOffset = UIPosition::getAnchorOffset(position.anchor, myElement->getLocalBoundsUpToComponent(sortIndex));

    updateVertices();
}

void ButtonComponent::update()
{   
    selectedLastFrame = selected;
    selected = isSelected();

    // Somewhat convoluted method of not allowing the button to be pressed the same frame it was selected.
    // This is designed to prevent misclicks by not letting the cursor be clicked and move onto the button and activate it.
    // Because of this, there must be a moment where the button is selected but not pressed before it can be activated.
    if (justSelected())
    {
        canPress = !isPressed();
    }
    else
    {
        if (isSelected())
        {
            if (!isPressed() && !canPress)
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
    pressed = isDown();

    // for changing textures, tracks new button state
    ButtonState nextState;
    if (pressed) nextState = ButtonState::DOWN;
    else if (isSelected()) nextState = ButtonState::HOVER;
    else nextState = ButtonState::UP;

    // if state changed, change visuals
    if (nextState != buttonState)
    {
        buttonState = nextState;

        updateVisuals();
    }
}

void ButtonComponent::draw()
{
    game->getWindow()->getWindow().draw(vertices.data(), vertices.size(), sf::PrimitiveType::Triangles, renderStates.texture);
}

void ButtonComponent::setButtonTextures(std::map<ButtonState, sf::Texture*> newTextures, sf::Vector2f size, bool sizeIsScale)
{
    textures = newTextures;

    this->size = toV2F(textures[buttonState]->getSize());

    if (sizeIsScale)
    {
        this->size.x *= size.x;
        this->size.y *= size.y;
    }
    else
    {
        this->size = size;
    }

    updateVisuals();
}

bool ButtonComponent::isSelected()
{
    sf::FloatRect gb = getGlobalBounds();

    return mouseRectCollide(game, gb.position, gb.size);
}

bool ButtonComponent::isDown()
{
    return isPressed() && canPress;
}

bool ButtonComponent::justPressed()
{
    return pressed && !pressedLastFrame;
}

void ButtonComponent::updateVertices()
{
    vertices = VertexGroup::createTriangleVerts(myElement->effectivePosition + position.position + originOffset + anchorOffset, size, sf::IntRect({0, 0}, toV2I(textures[buttonState]->getSize())));
    
    renderStates.texture = textures[buttonState];
}

bool ButtonComponent::isPressed()
{
    return isSelected() && game->getInput()->isKeyPressed("LEFTCLICK");
}

bool ButtonComponent::justSelected()
{
    return selected && !selectedLastFrame;
}