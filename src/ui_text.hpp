#pragma once

#include "ui_element.hpp"

#include <memory>
#include <string>

class Game;

class UIText : public UIElement
{
public:
    UIText();

    UIText(Game* game, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Font* font, std::string text, unsigned int fontSize, sf::Color fontColor = sf::Color(0, 0, 0), std::string parentName = "");

    void init(sf::Font* font, std::string text, unsigned int fontSize, sf::Color fontColor);

    // the full text
    void setText(std::string text);
    
    // use ### to represent the changeable value
    void setBaseText(std::string baseText);

    void setValue(std::string value);

    std::string getText();
    
    UIText* getAsText() override;

    void resize(sf::Vector2f newSize, int posSet = -1) override;

    void draw() override;
private:
    // no default constructor for sf::Text
    std::unique_ptr<sf::Text> displayText;

    std::string baseText;

    std::string value;
};