#include "text_ui_element.hpp"
#include "../core/game.hpp"
#include <cstdint>
#include <string>

TextUIElement::TextUIElement(Game* pf_game, sf::Vector2f pf_position, std::string p_data) : UIElement(pf_game, pf_position), m_text(*pf_game->getAssetManager()->getFont("sfml_font"), "PLACEHOLDER", 100)
{
    setData(p_data);
}

void TextUIElement::updateVisuals()
{
    m_text.setPosition(m_position);
}

void TextUIElement::draw()
{
    m_window->draw(m_text);
}

void TextUIElement::setCharacterSize(unsigned int p_characterSize)
{
    float l_sizeRatio = toFloat(p_characterSize) / toFloat(m_text.getCharacterSize());
    m_text.setScale({l_sizeRatio, l_sizeRatio});
}

void TextUIElement::processDataCommand_Child(std::string p_key, std::string p_value)
{
    if (p_key == "text")
    {
        m_text.setString(p_value);
    }
    if (p_key == "font")
    {
        sf::Font* font = m_game->getAssetManager()->getFont(p_value);
        if (font) m_text.setFont(*font);
    }
    if (p_key == "style")
    {
        std::uint32_t style = std::stoul(p_value);
        m_text.setStyle(style);
    }
    if (p_key == "character_size")
    {
        unsigned int size = std::stoul(p_value);
        setCharacterSize(size);
    }
}