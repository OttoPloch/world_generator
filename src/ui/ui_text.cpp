// #include "ui_text.hpp"
// #include "../core/game.hpp"

// UIText::UIText() : UIElement() {}

// UIText::UIText(Game* game, UILayer* uiLayer, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Font* font, std::string text, unsigned int fontSize, sf::Color fontColor, UIElement* parent) : UIElement(game, uiLayer, name, ID, posSet, position, {0.f, 0.f}, parent)
// {
//     init(font, text, fontSize, fontColor);
// }

// void UIText::init(Game* game, UILayer* uiLayer, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Font* font, std::string text, unsigned int fontSize, sf::Color fontColor, UIElement* parent)
// {
//     baseInit(game, uiLayer, name, ID, posSet, position, {0.f, 0.f}, parent);

//     init(font, text, fontSize, fontColor);
// }

// void UIText::init(sf::Font* font, std::string text, unsigned int fontSize, sf::Color fontColor)
// {
//     displayText = std::make_unique<sf::Text>(*font, text, fontSize);

//     displayText->setPosition({left(), top()});
//     displayText->setStyle(sf::Text::Bold);
//     displayText->setFillColor(fontColor);

//     value = "";

//     if (text.find("###") != std::string::npos)
//     {
//         setBaseText(text);
//     }
//     else
//     {
//         baseText = "";
//     }

//     updateSize();
// }

// sf::FloatRect UIText::getBoundingBox()
// {
//     return sf::FloatRect({left(), top()}, {size.x, size.y + 10.f});
// }

// void UIText::setText(std::string text)
// {
//     displayText->setString(text);

//     updateSize();
// }

// void UIText::setBaseText(std::string baseText)
// {
//     this->baseText = baseText;

//     std::string before = baseText.substr(0, baseText.find("###"));
//     std::string after = baseText.substr(baseText.find("###") + 3);

//     displayText->setString(before + value + after);

//     updateSize();
// }

// void UIText::setValue(std::string value)
// {
//     this->value = value;

//     if (baseText.find("###") != std::string::npos)
//     {
//         std::string before = baseText.substr(0, baseText.find("###"));
//         std::string after = baseText.substr(baseText.find("###") + 3);
    
//         displayText->setString(before + value + after);

//         updateSize();
//     }
//     else
//     {
//         std::cout << "ERROR setting value of ui text to " << value << ". Base text of '" << baseText << "' does not have ### to represent the value.\n";
//     }
// }

// std::string UIText::getText()
// {
//     return displayText->getString().toAnsiString();
// }

// UIText* UIText::getAsText() { return this; }

// void UIText::resize(sf::Vector2f newSize, int posSet)
// {
//     size = displayText->getGlobalBounds().size;
    
//     if (posSet != -1) this->posSet = posSet;

//     displayText->setPosition({left(), top()});
// }

// void UIText::draw()
// {
//     game->getWindow()->getWindow().draw(*displayText);
// }