#include "text_ui_element.hpp"
#include "../core/game.hpp"
#include <cstdint>
#include <string>

TextUIElement::TextUIElement(Game* pf_game, unsigned int pf_ID, std::string p_data, UIElement* pf_parent) : UIElement(pf_game, pf_ID, pf_parent), m_text(*pf_game->getAssetManager()->getFont("sfml_font"), "PLACEHOLDER", 100)
{
    setData(p_data);
}

sf::Vector2f TextUIElement::getSize()
{
    return m_text.getGlobalBounds().size;
}

void TextUIElement::draw(bool p_debug)
{
    // TEMP
    if (m_text.getString() == "X")
    {
        m_position.m_offset = m_game->getInputManager()->cursor->getGameCursorUIPosition();
        updateVisuals();
    }
    ///////

    m_window->draw(m_text);

    if (p_debug)
    {
        sf::RectangleShape rect(getSize());
        rect.setPosition(m_globalPosition);
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Blue);
        rect.setOutlineThickness(1.f);

        m_window->draw(rect);
    }
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
    else if (p_key == "font")
    {
        sf::Font* l_font = m_game->getAssetManager()->getFont(p_value);
        if (l_font) m_text.setFont(*l_font);
    }
    else if (p_key == "style")
    {
        std::uint32_t l_style = std::stoul(p_value);
        m_text.setStyle(l_style);
    }
    else if (p_key == "character_size")
    {
        unsigned int l_charSize = std::stoul(p_value);
        setCharacterSize(l_charSize);
    }
    else if (p_key == "color")
    {
        std::vector<float> l_values = getFloatsFromString(p_value, ", ");
        int l_colors[4] = {0, 0, 0, 255};

        for (int i = 0; i < std::min(static_cast<int>(l_values.size()), 4); i++)
        {
            l_colors[i] = static_cast<int>(l_values[i]);
        }

        m_text.setFillColor(sf::Color(l_colors[0], l_colors[1], l_colors[2], l_colors[3]));
    }
    else if (p_key == "outline_thickness")
    {
        float l_thickness = std::stof(p_value);

        m_text.setOutlineThickness(l_thickness);
    }
}

void TextUIElement::updateVisuals_Child()
{
    m_text.setOrigin(m_text.getLocalBounds().position);
    m_text.setPosition(m_globalPosition);
}