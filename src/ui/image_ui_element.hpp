#pragma once

#include "ui_element.hpp"
#include "../graphics/sprite.hpp"

class ImageUIElement : public UIElement
{
public:
    ImageUIElement(Game* pf_game, unsigned int pf_ID, std::string p_data, UIElement* pf_parent = nullptr);

    sf::Vector2f getSize() override;

    void update(float p_dt) override;

    void draw(bool p_debug) override;
private:
    void processDataCommand_Child(std::string p_key, std::string p_value) override;

    void updateVisuals_Child() override;

    Sprite m_sprite;
};