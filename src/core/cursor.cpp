#include "cursor.hpp"
#include "game.hpp"
#include "input.hpp"
#include "../ui/ui_element.hpp"
#include "../ui/components/image_component.hpp"

Cursor::Cursor() {}

Cursor::Cursor(Game* game) : game(game), input(game->getInput())
{
    gameCursorPosition = toV2F(game->getWindow()->getSize().x / 2, game->getWindow()->getSize().y / 2);

    cursorElement = game->getScene()->getUILayer()->createElement(std::make_unique<UIElement>(game, "__CURSOR", UIPosition(input->getCursorWindowPos()), INT32_MAX));
    cursorElement->addComponent<ImageComponent>(game, cursorElement, UIPosition({0, 0}), "CURSOR IMAGE", 0, game->getAssetManager()->getTexture("cursor"), sf::Vector2f(30, 30), false);
}

void Cursor::inputUpdate()
{
    // limits the game cursor to on the screen.
    sf::Vector2u windowSize = game->getWindow()->getSize();
    gameCursorPosition = {std::min(std::max(gameCursorPosition.x, 0.f), toFloat(windowSize.x)), std::min(std::max(gameCursorPosition.y, 0.f), toFloat(windowSize.y))};

    cursorElement->position.position = gameCursorPosition;
    
    if (input->getHideCursor()) cursorElement->visible = false;
    else cursorElement->visible = true;

    cursorElement->updateVisuals();
}

void Cursor::draw()
{

}