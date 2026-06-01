#include "text_component.hpp"
#include "../../core/game.hpp"
#include <SFML/Graphics/Rect.hpp>

TextComponent::TextComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, std::string text, sf::Font* font, unsigned int fontSize) : UIComponent(game, myElement, position, identifier, sortIndex)
{
    this->text = std::make_unique<sf::Text>(*font, text, 50);

    float sizeRatio = toFloat(fontSize) / toFloat(this->text->getCharacterSize());
    this->text->setScale({sizeRatio, sizeRatio});

    updateVisuals();
}

sf::FloatRect TextComponent::getLocalBounds()
{
    if (text)
    {
        return {position.position + originOffset + anchorOffset, text->getGlobalBounds().size};
    }
    else
    {
        return {{0, 0}, {0,0}};
    }
}

void TextComponent::updateVisuals()
{
    text->setOrigin(text->getLocalBounds().position);

    originOffset = UIPosition::getOriginOffset(position, text->getGlobalBounds().size);
    anchorOffset = UIPosition::getAnchorOffset(position, myElement->getLocalBoundsUpToComponent(sortIndex));

    text->setPosition(myElement->effectivePosition + position.position + originOffset + anchorOffset);
}

void TextComponent::draw()
{
    game->getWindow()->draw(*text);
}

void TextComponent::setText(std::string newText)
{
    text->setString(newText);

    updateVisuals();
}