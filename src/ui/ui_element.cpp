#include "ui_element.hpp"
#include "../core/game.hpp"
#include <SFML/System/Vector2.hpp>

UIElement::UIElement(Game* p_game, UIPosition p_position, UIElement* p_parent) : m_game(p_game), m_window(p_game->getWindow()), m_position(p_position), m_parent(p_parent)
{

}

void UIElement::setData(std::string p_data)
{
    std::vector<std::string> l_commands = getSegmentsFromString(p_data, "; ");

    for (auto i_command : l_commands)
    {
        processDataCommand(i_command);
    }
}

void UIElement::updateVisuals()
{
    m_position.setOriginOffset(this);
    m_position.setAnchorOffset(this);

    m_globalPosition = calculateGlobalPosition();

    updateVisuals_Child();
}

UIPosition UIElement::getUIPosition()
{
    return m_position;
}

sf::FloatRect UIElement::getGlobalBounds()
{
    return {m_globalPosition, getSize()};
}

sf::Vector2f UIElement::getSize()
{
    return {0, 0};
}

void UIElement::update()
{

}

void UIElement::draw(bool p_debug)
{

}

UIElement::~UIElement()
{

}

void UIElement::processDataCommand(std::string p_command)
{
    std::cout << "COMMAND: " << p_command << "\n";

    auto l_splitterIndex = p_command.find(": ");

    auto l_key = p_command.substr(0, l_splitterIndex);
    auto l_value = p_command.substr(l_splitterIndex + 2);

    processDataCommand_Child(l_key, l_value);
}

sf::Vector2f UIElement::calculateGlobalPosition()
{
    return m_position.m_anchorOffset + m_position.m_originOffset + m_position.m_offset;
}

void UIElement::updateVisuals_Child()
{

}