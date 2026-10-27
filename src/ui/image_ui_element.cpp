#include "image_ui_element.hpp"
#include "../core/game.hpp"

ImageUIElement::ImageUIElement(Game* pf_game, unsigned int pf_ID, std::string p_data, unsigned int pf_parent) : UIElement(pf_game, pf_ID, pf_parent)
{
    m_sprite = Sprite(GamePosition(m_game, {0, 0}), m_game->getAssetManager()->getTexture("bush.png"));

    setData(p_data);
}

sf::Vector2f ImageUIElement::getSize()
{
    return m_sprite.getSize();
}

void ImageUIElement::update(float p_dt)
{
    m_sprite.update(p_dt);
}

void ImageUIElement::draw(bool p_debug)
{
    m_sprite.draw(m_game->getWindow()->getWindow());
}

void ImageUIElement::processDataCommand_Child(std::string p_key, std::string p_value)
{
    if (!m_sprite.sprite) return;

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
    else if (p_key == "color")
    {
        std::vector<float> l_values = getFloatsFromString(p_value, ", ");
        int l_colors[4] = {255, 255, 255, 255};

        for (int i = 0; i < std::min(static_cast<int>(l_values.size()), 4); i++)
        {
            l_colors[i] = static_cast<int>(l_values[i]);
        }

        m_sprite.sprite->setColor(sf::Color(l_colors[0], l_colors[1], l_colors[2], l_colors[3]));
    }
    else if (p_key == "animation")
    {
        Animation* l_animation = m_game->getAssetManager()->getAnimation(p_value);

        if (!l_animation) return;

        m_sprite.animation = std::make_unique<Animation>(*l_animation);
    }
    else if (p_key == "animation_speed_mult")
    {
        float l_value = std::stof(p_value);

        m_sprite.animSpeedMult = l_value;
    }
}

void ImageUIElement::updateVisuals_Child()
{
    m_sprite.getPositionVar().setPosition(m_globalPosition + m_sprite.getSize() / 2.f);
    m_sprite.syncPos();
}