#pragma once

#include "ui_element.hpp"

#include <memory>

class Game;

class UIText : public UIElement
{
public:
    UIText();

    UIText(Game* game, UILayer* uiLayer, int ID, unsigned int posSet, sf::Vector2f position, sf::Font* font, std::string text, unsigned int fontSize, sf::Color fontColor = sf::Color(0, 0, 0), int parentID = -1);

    void init(sf::Font* font, std::string text, unsigned int fontSize, sf::Color fontColor);

    void setText(std::string text);
    
    std::string getText();
    
    void resize(sf::Vector2f newSize, int posSet = -1) override;

    void draw() override;
private:
    // no default constructor for sf::Text
    std::unique_ptr<sf::Text> displayText;
};