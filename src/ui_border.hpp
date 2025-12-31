#pragma once

#include "common.hpp"
#include "ui_element.hpp"
#include "tile_set.hpp"

class UIBorder : public UIElement
{
public:
    UIBorder();

    UIBorder(TileSet* tileSet, UILayer* uiLayer, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, int parentIndex = -1);

    void init(TileSet* tileSet);

    TileSet* getTileSet();
private:
    TileSet* tileSet;
};