#pragma once

#include "ui_element.hpp"
#include "../utils/utils.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <array>

class FrameUIElement : public UIElement
{
public:
    FrameUIElement(Game* pf_game, std::string p_data, UIElement* pf_parent = nullptr);

    sf::Vector2f getSize() override;

    void draw(bool p_debug) override;
private:
    void processDataCommand_Child(std::string p_key, std::string p_value) override;

    void updateVisuals_Child() override;

    std::array<sf::Vertex, 6> m_vertices;

    sf::Color m_color;
    sf::Vector2f m_size;
};