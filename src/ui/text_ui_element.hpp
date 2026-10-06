#pragma once

#include "ui_element.hpp"
#include "ui_position.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class TextUIElement : public UIElement
{
public:
    TextUIElement(Game* pf_game, std::string p_data, UIElement* pf_parent = nullptr);

    sf::Vector2f getSize() override;

    void draw(bool p_debug) override;
private:
    void setCharacterSize(unsigned int p_characterSize);

    void processDataCommand_Child(std::string p_key, std::string p_value) override;

    void updateVisuals_Child() override;

    sf::Text m_text;
};