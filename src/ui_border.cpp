#include "ui_border.hpp"

UIBorder::UIBorder() : UIElement() {}

UIBorder::UIBorder(TileSet* tileSet, UILayer* uiLayer, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, int parentIndex) : UIElement(uiLayer, posSet, position, size, parentIndex)
{
    init(tileSet);
}

void UIBorder::init(TileSet* tileSet)
{
    this->tileSet = tileSet;
}

TileSet* UIBorder::getTileSet() { return tileSet; }