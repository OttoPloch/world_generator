#include "frame_ui_element.hpp"
#include "../core/window.hpp"
#include "../graphics/vertex_group.hpp"

FrameUIElement::FrameUIElement(Game* pf_game, sf::Vector2f pf_position, std::string p_data) : UIElement(pf_game, pf_position), m_vertices(VertexGroup::createTriangleVerts({0, 0}, {0, 0}, sf::Color::Black))
{
    setData(p_data);
}

void FrameUIElement::updateVisuals()
{

}

void FrameUIElement::draw()
{
    m_window->getWindow().draw(m_vertices.data(), m_vertices.size(), sf::PrimitiveType::Triangles);
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

        m_vertices = VertexGroup::createTriangleVerts(m_position, m_size, m_color);
    }
    else if (p_key == "color")
    {
        std::vector<float> l_values = getValuesFromString(p_value, ", ");
        int l_colors[3] = {0, 0, 0};

        for (int i = 0; i < std::min(static_cast<int>(l_values.size()), 3); i++)
        {
            l_colors[i] = static_cast<int>(l_values[i]);
        }

        m_color = sf::Color(l_colors[0], l_colors[1], l_colors[2]);

        for (auto& i_vertex : m_vertices)
        {
            i_vertex.color = m_color;
        }
    }
}