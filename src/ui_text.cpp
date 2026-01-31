#include "ui_text.hpp"
#include "game.hpp"

UIText::UIText() : UIElement() {}

UIText::UIText(Game* game, UILayer* uiLayer, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Font* font, std::string text, unsigned int fontSize, sf::Color fontColor, UIElement* parent) : UIElement(game, uiLayer, name, ID, posSet, position, {0.f, 0.f}, parent)
{
    init(font, text, fontSize, fontColor);
}

void UIText::init(Game* game, UILayer* uiLayer, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Font* font, std::string text, unsigned int fontSize, sf::Color fontColor, UIElement* parent)
{
    baseInit(game, uiLayer, name, ID, posSet, position, {0.f, 0.f}, parent);

    init(font, text, fontSize, fontColor);
}

void UIText::init(sf::Font* font, std::string text, unsigned int fontSize, sf::Color fontColor)
{
    displayText = std::make_unique<sf::Text>(*font, text, fontSize);

    displayText->setPosition({left(), top()});
    displayText->setStyle(sf::Text::Bold);
    displayText->setFillColor(fontColor);

    value = "";
    if (text.find("###") != std::string::npos)
    {
        setBaseText(text);
    }
    else
    {
        baseText = "";
    }

    updateSize();
}

void UIText::setText(std::string text)
{
    displayText->setString(text);

    updateSize();
}

void UIText::setBaseText(std::string baseText)
{
    this->baseText = baseText;

    std::string before = baseText.substr(0, baseText.find("###"));
    std::string after = baseText.substr(baseText.find("###") + 3);

    displayText->setString(before + value + after);

    updateSize();
}

void UIText::setValue(std::string value)
{
    this->value = value;

    if (baseText.find("###") != std::string::npos)
    {
        std::string before = baseText.substr(0, baseText.find("###"));
        std::string after = baseText.substr(baseText.find("###") + 3);
    
        displayText->setString(before + value + after);

        updateSize();
    }
    else
    {
        std::cout << "ERROR setting value of ui text to " << value << ". Base text of '" << baseText << "' does not have ### to represent the value.\n";
    }
}

std::string UIText::getText()
{
    return displayText->getString().toAnsiString();
}

UIText* UIText::getAsText() { return this; }

void UIText::resize(sf::Vector2f newSize, int posSet)
{
    size = newSize;
    
    if (posSet != -1) this->posSet = posSet;
    
    sf::Vector2f textTL = {0, 0};
    sf::Vector2f textAdjustedTL = textTL + displayText->getLocalBounds().position;
    sf::Vector2f roundedTL = {std::round(textAdjustedTL.x), std::round(textAdjustedTL.y)};

    sf::Vector2f textTR = {displayText->getGlobalBounds().size.x, 0};
    sf::Vector2f textAdjustedTR = textTR + displayText->getGlobalBounds().position;
    sf::Vector2f roundedTR = {std::round(textAdjustedTR.x), std::round(textAdjustedTR.y)};

    sf::Vector2f textBL = {0, displayText->getGlobalBounds().size.y};
    sf::Vector2f textAdjustedBL = textBL + displayText->getLocalBounds().position;
    sf::Vector2f roundedBL = {std::round(textAdjustedBL.x), std::round(textAdjustedBL.y)};

    sf::Vector2f textBR = displayText->getGlobalBounds().size;
    sf::Vector2f textAdjustedBR = textBR + displayText->getLocalBounds().position;
    sf::Vector2f roundedBR = {std::round(textAdjustedBR.x), std::round(textAdjustedBR.y)};
    
    sf::Vector2f textCenter = displayText->getGlobalBounds().size / 2.f;
    sf::Vector2f textAdjustedCenter = textCenter + displayText->getLocalBounds().position;
    sf::Vector2f roundedCenter = {std::round(textAdjustedCenter.x), std::round(textAdjustedCenter.y)};

    switch (this->posSet)
    {
        case 0:
            displayText->setOrigin(roundedTL);
            break;
        case 1:
            displayText->setOrigin(roundedTR);
            break;
        case 2:
            displayText->setOrigin(roundedBL);
            break;
        case 3:
            displayText->setOrigin(roundedBR);
            break;
        case 4:
            displayText->setOrigin(roundedCenter);
            break;
        case 5:
            displayText->setOrigin({roundedCenter.x, roundedTL.y});
            break;
        case 6:
            displayText->setOrigin({roundedTL.x, roundedCenter.y});
            break;
        case 7:
            displayText->setOrigin({roundedTR.x, roundedCenter.y});
            break;
        case 8:
            displayText->setOrigin({roundedCenter.x, roundedBL.y});
            break;
        default:
            displayText->setOrigin(roundedCenter);
            break;
    }

    displayText->setPosition({left(), top()});
}

void UIText::draw()
{
    game->getWindow()->getWindow().draw(*displayText);
}