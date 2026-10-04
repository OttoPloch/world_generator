#pragma once

#include "ui_element.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class TextUIElement : public UIElement
{
public:
    TextUIElement(Game* pf_game, sf::Vector2f pf_position, std::string p_data);

    void updateVisuals() override;

    void draw() override;
private:
    void setCharacterSize(unsigned int p_characterSize);

    void processDataCommand_Child(std::string p_key, std::string p_value) override;

    sf::Text m_text;
};