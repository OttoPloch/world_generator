#pragma once

#include "ui_element.hpp"

#include <memory>

class Game;

class UIText : public UIElement
{
public:
    UIText();

    UIText(Game* game, sf::Font* font, std::string text, unsigned int fontSize, UILayer* uiLayer, unsigned int posSet, sf::Vector2f position, int parentIndex = -1);

    void init(Game* game, sf::Font* font, std::string text, unsigned int fontSize);

    void setText(std::string text);
    
    std::string getText();
    
    void resize(sf::Vector2f newSize, int posSet = -1) override;

    void draw() override;
private:
    Game* game;

    // no default constructor for sf::Text
    std::unique_ptr<sf::Text> displayText;
};