#include "ui_layer.hpp"
#include "text_ui_element.hpp"
#include "../core/game.hpp"

UILayer::UILayer(Game* p_game, Camera* p_camera) : m_game(p_game), m_camera(p_camera), m_IDCounter(0)
{
    addElement(std::make_unique<TextUIElement>(m_game, sf::Vector2f(0, 0), "text: Hello, World!; font: sfml_font; style: 0; character_size: 24; color: 255, 0, 0; "));
    addElement(std::make_unique<TextUIElement>(m_game, sf::Vector2f(0, 20), "text: Hello, World!; font: sfml_font; style: 1; character_size: 24; color: 255, 130, 0; "));
    addElement(std::make_unique<TextUIElement>(m_game, sf::Vector2f(0, 40), "text: Hello, World!; font: sfml_font; style: 2; character_size: 24; color: 255, 255, 0; "));
    addElement(std::make_unique<TextUIElement>(m_game, sf::Vector2f(0, 60), "text: Hello, World!; font: sfml_font; style: 4; character_size: 24; color: 0, 255, 0; "));
    addElement(std::make_unique<TextUIElement>(m_game, sf::Vector2f(0, 80), "text: Hello, World!; font: sfml_font; style: 8; character_size: 24; color: 0, 0, 255; "));
    addElement(std::make_unique<TextUIElement>(m_game, sf::Vector2f(0, 100), "text: Hello, World!; font: sfml_font; style: 15; character_size: 24; color: 255, 0, 255; "));

    updateVisuals();
}

void UILayer::addElement(std::unique_ptr<UIElement> p_newElement)
{
    if (p_newElement)
    {
        m_elements[getNewID()] = std::move(p_newElement);
    }
}

UIElement* UILayer::getElement(unsigned int ID)
{
    if (m_elements.find(ID) != m_elements.end())
    {
        return m_elements[ID].get();
    }

    return nullptr;
}

void UILayer::updateVisuals()
{
    setUIViewSize();
    UIView.setCenter({UIView.getSize().x / 2.f, UIView.getSize().y / 2.f});
    
    for (auto& i_entry : m_elements)
    {
        i_entry.second->updateVisuals();
    }
}

void UILayer::update(float p_dt)
{
    for (auto& i_entry : m_elements)
    {
        i_entry.second->update();
    }
}

void UILayer::tick()
{

}

void UILayer::draw(bool p_debug)
{
    m_game->getWindow()->setView(UIView);

    for (auto& i_entry : m_elements)
    {
        i_entry.second->draw();
    }

    m_game->getWindow()->setView(m_camera->getView());
}

unsigned int UILayer::getNewID()
{
    m_IDCounter++;

    return m_IDCounter - 1;
}

void UILayer::setUIViewSize()
{
    float l_baseSize = std::fmin(1080.f, m_game->getWindow()->getSize().y);
    
    UIView.setSize({l_baseSize * m_game->getWindow()->getAspectRatio(), l_baseSize});
}