#include "ui_element.hpp"
#include "../core/game.hpp"

UIElement::UIElement(Game* p_game, sf::Vector2f p_position) : m_game(p_game), m_window(p_game->getWindow())
{
    setPosition(p_position);
}

sf::Vector2f UIElement::getPosition()
{
    return m_position;
}

void UIElement::setPosition(sf::Vector2f p_newPosition)
{
    m_position = p_newPosition;
}

void UIElement::setData(std::string p_data)
{
    std::vector<std::string> l_commands = getSegmentsFromString(p_data, "; ");

    for (auto i_command : l_commands)
    {
        processDataCommand(i_command);
    }
}

void UIElement::update()
{

}

void UIElement::draw()
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