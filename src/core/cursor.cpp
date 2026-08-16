#include "cursor.hpp"
#include "game.hpp"
#include "input.hpp"
#include "../ui/ui_element.hpp"
#include "../ui/components/image_component.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Mouse.hpp>
#include "../ui/ui_layer.hpp"
#include "../entities/components/control_component.hpp"

Cursor::Cursor(Game* game, std::string alternativeKeyForLeftClick, std::string alternativeKeyForRightClick) : game(game), input(game->getInput()), alternativeKeyForLeftClick(alternativeKeyForLeftClick), alternativeKeyForRightClick(alternativeKeyForRightClick)
{
    sf::Vector2u windowSize = game->getWindow()->getSize();
    gameCursorPosition = toV2F(windowSize.x / 2, windowSize.y / 2);

    cursorElement = game->getScene()->getUILayer()->createElement(std::make_unique<UIElement>(game, "__CURSOR", UIPosition(gameCursorPosition), INT32_MAX));
    cursorElement->addComponent<ImageComponent>(game, cursorElement, UIPosition({0, 0}), "CURSOR IMAGE", 0, game->getAssetManager()->getTexture("cursor"), sf::Vector2f(30, 30), false);

    UIMode = false;
    usingMovementForUISelector = false;
    mouseMovedThisFrame = false;

    UISelector = game->getScene()->getUILayer()->createElement(std::make_unique<UIElement>(game, "__UI SELECTOR", UIPosition({windowSize.x / 2.f, windowSize.y / 2.f}), INT32_MAX, nullptr));
    UISelector->addComponent<BackgroundComponent>(game, UISelector, UIPosition({0, 0}), "SELECTOR BG", 0, sf::Vector2f(30, 30), 2, game->getAssetManager()->getTexture("white_border", "texture_atlases/ui/"), game->getAssetManager()->getTextureAtlas("background_8px", "ui/"), false);
    UISelector->visible = false;
    selectedElement = nullptr;
    selectedComponent = nullptr;
}

void Cursor::inputUpdate(float dt)
{
    if (input->getControl("MENU"))
    {
        UIMode = !UIMode;

        if (UIMode)
        {
            cursorElement->visible = false;

            selectedEntity = nullptr;
            selectedTile = nullptr;
        }
        else
        {
            cursorElement->visible = true;
        }
    }

    if (UIMode)
    {
        if (selectedComponent) UISelector->visible = true;

        usingMovementForUISelector = true;
        sf::Vector2f movement = input->getMovement();
        usingMovementForUISelector = false;

        if (UIMoveClock.getElapsedTime().asSeconds() > game->getSettings()->input_UISelectorMoveCooldown)
        {
            if (movement != sf::Vector2f(0, 0))
            {
                moveUISelector(movement);

                UIMoveClock.restart();
            }
        }
    }
    else
    {
        UISelector->visible = false;

        if (canDoInputType("TYPE:WORLD"))
        {
            // TODO: give entities a hitbox, use that for actions and selecting.
            // This will makes entities without a hitbox not selectable, ideal for (and this is just an idea that popped up as an example so dont take it too seriously)
            // butterflies or something that is just visual and moves and animates but is not interactable (could be but for the example it's not). Also, that would make it
            // necessary to give different entities different z values, instead of just using entityTileZEquivalent in Settings, so that butterflies can be above all tiles.
            selectedEntity = game->getScene()->getEntityLayer()->getEntityAtPos(getGameCursorCoords(), false);
            selectedTile = game->getScene()->getChunkLayer()->getTileAtPosition(getGameCursorCoords(), true);
    
            if (selectedTile && selectedEntity)
            {
                if (selectedTile->z > game->getSettings()->entityTileZEquivalent) selectedEntity = nullptr;
                else selectedTile = nullptr;
            }
        }
        else
        {
            selectedEntity = nullptr;
            selectedTile = nullptr;
        }
    }

    if (game->getWindow()->getWindow().hasFocus())
    {
        // TEMP, need to fully implement controller support in a neat way
        if (sf::Joystick::isConnected(0) && !mouseMovedThisFrame)
        {
            if (!UIMode)
            {
                sf::Vector2f cursorMovement = {
                    input->getAxis(sf::Joystick::Axis::U) * game->getSettings()->input_controllerCursorSensitivity * dt,
                    input->getAxis(sf::Joystick::Axis::V) * game->getSettings()->input_controllerCursorSensitivity * dt
                };
    
                gameCursorPosition.x += cursorMovement.x;
                gameCursorPosition.y += cursorMovement.y;
            }
        }
        else
        {
            gameCursorPosition = toV2F(sf::Mouse::getPosition(game->getWindow()->getWindow()));
        }
    }

    // limits the game cursor to on the screen.
    sf::Vector2u windowSize = game->getWindow()->getSize();
    gameCursorPosition = {std::min(std::max(gameCursorPosition.x, 0.f), toFloat(windowSize.x)), std::min(std::max(gameCursorPosition.y, 0.f), toFloat(windowSize.y))};

    cursorElement->position.position = gameCursorPosition;

    cursorElement->updateVisuals();

    mouseMovedThisFrame = false;
}

void Cursor::mouseMoveEvent(sf::Event::MouseMoved mouseMoved)
{
    mouseMovedThisFrame = true;
}

void Cursor::moveUISelector(sf::Vector2f direction)
{
    UIComponent* newComponent = nullptr;
    if (selectedElement)
    {
        // finding a new component to select within the selected element.

        newComponent = selectedElement->getNearestComponent(direction, selectedComponent);
    }

    if (!newComponent)
    {
        // no options in the selected element or none are selected, finding a new element

        UIElement* newElement = game->getScene()->getUILayer()->getNearestElement(direction, selectedElement);
    
        if (!newElement) return;
    
        selectedElement = newElement;
        selectedComponent = newElement->getNearestComponent(direction, nullptr);

        if (!selectedComponent) return;

        UISelector->parent = newElement;
    }
    else
    {
        selectedComponent = newComponent;
    }

    sf::FloatRect compGB = selectedComponent->getGlobalBounds();

    UISelector->position.position = compGB.position - selectedElement->getGlobalBounds().position;
    UISelector->getComponent<BackgroundComponent>()->resize(compGB.size);

    UISelector->updateVisuals();
}

bool Cursor::isUIModeActive()
{
    return UIMode;
}

bool Cursor::isUsingMovementForUISelector()
{
    return usingMovementForUISelector;
}

sf::Vector2f Cursor::getGameCursorPosition()
{
    return gameCursorPosition;
}

sf::Vector2f Cursor::getGameCursorCoords()
{
    return game->getWindow()->getWindow().mapPixelToCoords(sf::Vector2i(gameCursorPosition));
}

UIElement* Cursor::getSelectedElement()
{
    return selectedElement;
}

UIComponent* Cursor::getSelectedComponent()
{
    return selectedComponent;
}

Entity* Cursor::getSelectedEntity()
{
    return selectedEntity;
}

Tile* Cursor::getSelectedTile()
{
    return selectedTile;
}

UIElement* Cursor::getCursorElement()
{
    return cursorElement;
}

bool Cursor::processMouseButtonEvent(sf::Event::MouseButtonPressed mouseButtonPressed, std::string& responseInput)
{
    if (mouseButtonPressed.button == sf::Mouse::Button::Left)
    {
        if (UIMode)
        {
            responseInput = "UI LEFTCLICK";
        }
        else
        {
            responseInput = "LEFTCLICK";
        }

        return true;
    }
    else if (mouseButtonPressed.button == sf::Mouse::Button::Right)
    {
        responseInput = "RIGHTCLICK";

        return true;
    }
    
    return false;
}

bool Cursor::canDoInputType(std::string type)
{
    if (type == "TYPE:WORLD")
    {
        if (!game->getScene()->getEntityLayer()->player || !game->getScene()->getEntityLayer()->player->getComponent<ControlComponent>())
        {
            return false;
        }

        if (game->getScene()->getUILayer()->checkUICollision() || UIMode)
        {
            return false;
        }
    }
    else if (type == "TYPE:UI")
    {
        if (!game->getScene()->getUILayer()->checkUICollision() && !UIMode)
        {
            return false;
        }
    }

    return true;
}

bool Cursor::getMouseClick(sf::Mouse::Button mouseButton)
{
    if (mouseButton == sf::Mouse::Button::Left)
    {
        bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        
        if (alternativeKeyForLeftClick == "NONE")
        {
            if (isPressed) return true;
        }
        else
        {
            if (!UIMode && isPressed) return true;
            if (UIMode && input->isKeyPressed(alternativeKeyForLeftClick)) return true;
        }
    }
    else if (mouseButton == sf::Mouse::Button::Right)
    {
        bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

        if (alternativeKeyForRightClick == "NONE")
        {
            if (isPressed) return true;
        }
        else
        {
            if (!UIMode && isPressed) return true;
            if (UIMode && input->isKeyPressed(alternativeKeyForRightClick)) return true;
        }
    }

    return false;
}