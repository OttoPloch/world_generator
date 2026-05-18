#pragma once

#include "ui_component.hpp"
#include "../../core/common.hpp"
#include <SFML/Graphics/Rect.hpp>

struct TextComponent : public UIComponent
{
    TextComponent(Game* game, UIElement* myElement, UIPosition, std::string text, sf::Font* font, unsigned int fontSize);

    sf::FloatRect getLocalBounds() override;

    void updateVisuals() override;

    void draw() override;

    std::unique_ptr<sf::Text> text;
};