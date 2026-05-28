#pragma once

#include "../ui_position.hpp"

class Game;
class UIElement;

struct UIComponent
{
    UIComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex);

    sf::FloatRect getGlobalBounds();

    virtual sf::FloatRect getLocalBounds();

    virtual void updateVisuals();

    virtual void draw();

    Game* game;
    UIElement* myElement;

    UIPosition position;
    sf::Vector2f originOffset;
    sf::Vector2f anchorOffset;

    std::string identifier;

    // UIElement component vector is sorted by this.
    // This is used to calculate the size of the element
    // with every component up to this one (skipping any with
    // the same index). By doing this, components can be aligned with
    // other components by the anchor without having to account for
    // every component, which would place the anchor on the very
    // edge of the element.
    int sortIndex;
};