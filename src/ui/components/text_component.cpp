#include "text_component.hpp"
#include "../../core/game.hpp"
#include <SFML/Graphics/Rect.hpp>

TextComponent::TextComponent(Game* game, UIElement* myElement, UIPosition position, std::string text, sf::Font* font, unsigned int fontSize) : UIComponent(game, myElement, position)
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
        return {position.position - text->getOrigin(), text->getLocalBounds().size};
    }
    else
    {
        return {{0, 0}, {0,0}};
    }
}

void TextComponent::updateVisuals()
{
    text->setPosition(myElement->position.position + position.position);
}

void TextComponent::draw()
{
    game->getWindow()->draw(*text);
}