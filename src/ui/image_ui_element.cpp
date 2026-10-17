#include "image_ui_element.hpp"
#include "../core/game.hpp"

ImageUIElement::ImageUIElement(Game* pf_game, unsigned int pf_ID, std::string p_data, UIElement* pf_parent) : UIElement(pf_game, pf_ID, p_data, pf_parent)
{
    // initialize the sprite.
}

sf::Vector2f ImageUIElement::getSize()
{
    return m_sprite.getSize();
}

void ImageUIElement::draw(bool p_debug)
{
    m_sprite.draw(m_game->getWindow()->getWindow());
}

void ImageUIElement::processDataCommand_Child(std::string p_key, std::string p_value)
{
    if (p_key == "texture")
    {
        sf::Texture* l_newTexture = m_game->getAssetManager()->getTexture("", p_value, true);
        
        if (!l_newTexture) return;

        m_sprite.setTexture(l_newTexture);
    }
    else if (p_key == "size")
    {
        std::vector<float> l_values = getFloatsFromString(p_value, ", ");
        float x = 0, y = 0;
        bool l_isScale = false;

        if (l_values.size() != 2 && l_values.size() != 3) return;

        x = l_values[0];
        y = l_values[1];
        
        if (l_values.size() == 3) l_isScale = static_cast<int>(l_values[2]);

        m_sprite.resize({x, y}, l_isScale);
    }
}

void ImageUIElement::updateVisuals_Child()
{
    m_sprite.syncPos();
}