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
        sf::Font* l_font = m_game->getAssetManager()->getFont(p_value);
        if (l_font) m_text.setFont(*l_font);
    }
    if (p_key == "style")
    {
        std::uint32_t l_style = std::stoul(p_value);
        m_text.setStyle(l_style);
    }
    if (p_key == "character_size")
    {
        unsigned int l_charSize = std::stoul(p_value);
        setCharacterSize(l_charSize);
    }
    if (p_key == "color")
    {
        int colors[3] = {0, 0, 0};

        std::string l_valueTrimmed = p_value;
        for (int i = 0; i < 3; i++)
        {
            auto l_commaIndex = l_valueTrimmed.find(", ");
            if (l_commaIndex == std::string::npos)
            {
                colors[i] = std::stoi(l_valueTrimmed);
                break;
            }

            colors[i] = std::stoi(l_valueTrimmed.substr(0, l_commaIndex));
            
            l_valueTrimmed.erase(0, l_commaIndex + 2);
        }

        m_text.setFillColor(sf::Color(colors[0], colors[1], colors[2]));
    }
}