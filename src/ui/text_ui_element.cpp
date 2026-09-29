#include "text_ui_element.hpp"
#include "../core/game.hpp"

TextUIElement::TextUIElement(Game* pf_game, sf::Vector2f pf_position, std::string p_text, sf::Font* p_font, unsigned int p_characterSize) : UIElement(pf_game, pf_position), m_text(*p_font, p_text, 100)
{
    float l_sizeRatio = toFloat(p_characterSize) / toFloat(m_text.getCharacterSize());
    m_text.setScale({l_sizeRatio, l_sizeRatio});
}

void TextUIElement::setText(std::string p_newText)
{
    m_text.setString(p_newText);
}

void TextUIElement::draw()
{
    m_window->draw(m_text);
}