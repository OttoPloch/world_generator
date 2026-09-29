#include "ui_layer.hpp"
#include "text_ui_element.hpp"
#include "../core/game.hpp"

UILayer::UILayer(Game* p_game, Camera* p_camera) : m_game(p_game), m_camera(p_camera)
{
    addElement(std::make_unique<TextUIElement>(m_game, sf::Vector2f(100, 100), "Hello, World!", m_game->getAssetManager()->getFont("sfml_font"), 24));

    updateVisuals();
}

void UILayer::addElement(std::unique_ptr<UIElement> p_newElement)
{
    if (p_newElement)
    {
        m_elements[getNewID()] = std::move(p_newElement);
    }
}

void UILayer::updateVisuals()
{
    setUIViewSize();
    UIView.setCenter({UIView.getSize().x / 2.f, UIView.getSize().y / 2.f});
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