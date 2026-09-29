#include "ui_element.hpp"
#include "../core/game.hpp"

UIElement::UIElement(Game* p_game, sf::Vector2f p_position) : m_game(p_game), m_window(p_game->getWindow()), m_position(p_position)
{
    
}

sf::Vector2f UIElement::getPosition()
{
    return m_position;
}

void UIElement::setPosition(sf::Vector2f p_newPosition)
{
    m_position = p_newPosition;
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