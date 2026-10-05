#include "frame_ui_element.hpp"
#include "../core/window.hpp"
#include "../graphics/vertex_group.hpp"
#include "ui_position.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

FrameUIElement::FrameUIElement(Game* pf_game, UIPosition pf_position, std::string p_data, UIElement* pf_parent) : UIElement(pf_game, pf_position, pf_parent), m_vertices(VertexGroup::createTriangleVerts({0, 0}, {0, 0}, sf::Color::Black))
{
    setData(p_data);
}

sf::Vector2f FrameUIElement::getSize()
{
    return m_size;
}

void FrameUIElement::draw(bool p_debug)
{
    m_window->getWindow().draw(m_vertices.data(), m_vertices.size(), sf::PrimitiveType::Triangles);

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

void FrameUIElement::processDataCommand_Child(std::string p_key, std::string p_value)
{
    if (p_key == "size")
    {
        std::vector<float> l_values = getValuesFromString(p_value, ", ");
        float x = 0, y = 0;

        if (l_values.size() == 2)
        {
            x = l_values[0];
            y = l_values[1];
        }

        m_size = {x, y};
    }
    else if (p_key == "color")
    {
        std::vector<float> l_values = getValuesFromString(p_value, ", ");
        int l_colors[4] = {0, 0, 0, 255};

        for (int i = 0; i < std::min(static_cast<int>(l_values.size()), 4); i++)
        {
            l_colors[i] = static_cast<int>(l_values[i]);
        }

        m_color = sf::Color(l_colors[0], l_colors[1], l_colors[2], l_colors[3]);

        for (auto& i_vertex : m_vertices)
        {
            i_vertex.color = m_color;
        }
    }
}

void FrameUIElement::updateVisuals_Child()
{
    m_vertices = VertexGroup::createTriangleVerts(m_globalPosition, m_size, m_color);
}