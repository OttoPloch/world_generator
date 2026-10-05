#include "ui_layer.hpp"
#include "text_ui_element.hpp"
#include "frame_ui_element.hpp"
#include "../core/game.hpp"
#include "ui_position.hpp"

UILayer::UILayer(Game* p_game, Camera* p_camera) : m_game(p_game), m_camera(p_camera), m_IDCounter(0)
{
    addElement(std::make_unique<TextUIElement>(m_game, UIPosition({0, 0}), "text: Hello, World!; font: sfml_font; style: 0; character_size: 24; color: 255, 0, 0"));
    addElement(std::make_unique<TextUIElement>(m_game, UIPosition({0, 20}), "text: Hello, World!; font: sfml_font; style: 1; character_size: 24; color: 255, 130, 0"));
    addElement(std::make_unique<TextUIElement>(m_game, UIPosition({0, 40}), "text: Hello, World!; font: sfml_font; style: 2; character_size: 24; color: 255, 255, 0"));
    addElement(std::make_unique<TextUIElement>(m_game, UIPosition({0, 60}), "text: Hello, World!; font: sfml_font; style: 4; character_size: 24; color: 0, 255, 0"));
    addElement(std::make_unique<TextUIElement>(m_game, UIPosition({0, 80}), "text: Hello, World!; font: sfml_font; style: 8; character_size: 24; color: 0, 0, 255"));
    addElement(std::make_unique<TextUIElement>(m_game, UIPosition({0, 100}), "text: Hello, World!; font: sfml_font; style: 15; character_size: 24; color: 255, 0, 255"));

    addElement(std::make_unique<FrameUIElement>(m_game, UIPosition({0, 120}), "size: 20, 20; color: 255, 255, 255"));
    addElement(std::make_unique<FrameUIElement>(m_game, UIPosition({20, 120}), "size: 20, 20; color: 200, 200, 200"));
    addElement(std::make_unique<FrameUIElement>(m_game, UIPosition({40, 120}), "size: 20, 20; color: 150, 150, 150"));
    addElement(std::make_unique<FrameUIElement>(m_game, UIPosition({60, 120}), "size: 20, 20; color: 100, 100, 100"));
    addElement(std::make_unique<FrameUIElement>(m_game, UIPosition({80, 120}), "size: 20, 20; color: 50, 50, 50"));
    addElement(std::make_unique<FrameUIElement>(m_game, UIPosition({100, 120}), "size: 20, 20; color: 0, 0, 0"));

    UIElement* t_parent = addElement(std::make_unique<FrameUIElement>(m_game, UIPosition({200, 10}), "size: 100, 100; color: 255, 255, 255"));

    addElement(std::make_unique<TextUIElement>(m_game, UIPosition({0, 0}, UIOrigin::TOP_LEFT, UIAnchor::TOP_LEFT), "text: 1; character_size: 16; color: 255, 0, 0", t_parent));
    addElement(std::make_unique<TextUIElement>(m_game, UIPosition({0, 0}, UIOrigin::TOP, UIAnchor::TOP), "text: 2; character_size: 16; color: 255, 0, 0", t_parent));
    addElement(std::make_unique<TextUIElement>(m_game, UIPosition({0, 0}, UIOrigin::TOP_RIGHT, UIAnchor::TOP_RIGHT), "text: 3; character_size: 16; color: 255, 0, 0", t_parent));
    addElement(std::make_unique<TextUIElement>(m_game, UIPosition({0, 0}, UIOrigin::LEFT, UIAnchor::LEFT), "text: 4; character_size: 16; color: 255, 0, 0", t_parent));
    addElement(std::make_unique<TextUIElement>(m_game, UIPosition({0, 0}, UIOrigin::CENTER, UIAnchor::CENTER), "text: 5; character_size: 16; color: 255, 0, 0", t_parent));
    addElement(std::make_unique<TextUIElement>(m_game, UIPosition({0, 0}, UIOrigin::RIGHT, UIAnchor::RIGHT), "text: 6; character_size: 16; color: 255, 0, 0", t_parent));
    addElement(std::make_unique<TextUIElement>(m_game, UIPosition({0, 0}, UIOrigin::BOTTOM_LEFT, UIAnchor::BOTTOM_LEFT), "text: 7; character_size: 16; color: 255, 0, 0", t_parent));
    addElement(std::make_unique<TextUIElement>(m_game, UIPosition({0, 0}, UIOrigin::BOTTOM, UIAnchor::BOTTOM), "text: 8; character_size: 16; color: 255, 0, 0", t_parent));
    addElement(std::make_unique<TextUIElement>(m_game, UIPosition({0, 0}, UIOrigin::BOTTOM_RIGHT, UIAnchor::BOTTOM_RIGHT), "text: 9; character_size: 16; color: 255, 0, 0", t_parent));

    addElement(std::make_unique<TextUIElement>(m_game, UIPosition({100, 100}, UIOrigin::CENTER), "text: X; font: sfml_font; character_size: 16; color: 255, 255, 255; outline_thickness: 10"));

    updateVisuals();
}

sf::Vector2f UILayer::getUIViewSize()
{
    return UIView.getSize();
}

UIElement* UILayer::addElement(std::unique_ptr<UIElement> p_newElement)
{
    if (p_newElement)
    {
        unsigned int l_newID = getNewID();

        m_elements[l_newID] = std::move(p_newElement);

        return m_elements[l_newID].get();
    }

    return nullptr;
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
        i_entry.second->draw(p_debug);
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