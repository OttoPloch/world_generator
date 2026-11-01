#include "ui_layer.hpp"
#include "image_ui_element.hpp"
#include "text_ui_element.hpp"
#include "frame_ui_element.hpp"
#include "../core/game.hpp"
#include "ui_position.hpp"
#include <algorithm>

UILayer::UILayer(Game* p_game, Camera* p_camera) : m_IDCounter(0), m_game(p_game), m_camera(p_camera), m_uiManagementSystem(m_game)
{
    addElement<TextUIElement>("position: 0, 0; text: Hello, World!; font: sfml_font; style: 0; character_size: 24; color: 255, 0, 0");
    addElement<TextUIElement>("position: 0, 20; text: Hello, World!; font: sfml_font; style: 1; character_size: 24; color: 255, 130, 0");
    addElement<TextUIElement>("position: 0, 40; text: Hello, World!; font: sfml_font; style: 2; character_size: 24; color: 255, 255, 0");
    addElement<TextUIElement>("position: 0, 60; text: Hello, World!; font: sfml_font; style: 4; character_size: 24; color: 0, 255, 0");
    addElement<TextUIElement>("position: 0, 80; text: Hello, World!; font: sfml_font; style: 8; character_size: 24; color: 0, 0, 255");
    addElement<TextUIElement>("position: 0, 100; text: Hello, World!; font: sfml_font; style: 15; character_size: 24; color: 255, 0, 255");

    addElement<FrameUIElement>("position: 0, 120; size: 20, 20; color: 255, 255, 255");
    addElement<FrameUIElement>("position: 20, 120; size: 20, 20; color: 200, 200, 200");
    addElement<FrameUIElement>("position: 40, 120; size: 20, 20; color: 150, 150, 150");
    addElement<FrameUIElement>("position: 60, 120; size: 20, 20; color: 100, 100, 100");
    addElement<FrameUIElement>("position: 80, 120; size: 20, 20; color: 50, 50, 50");
    addElement<FrameUIElement>("position: 100, 120; size: 20, 20; color: 0, 0, 0");

    unsigned int t_parentID = addElement<FrameUIElement>("position: 200, 10; size: 100, 100; color: 255, 255, 255; layer: 0")->getID();

    addElement<TextUIElement>("position: 0, 0, 0, 0; text: 1; character_size: 16; color: 255, 0, 0", t_parentID);
    addElement<TextUIElement>("position: 0, 0, 1, 1; text: 2; character_size: 16; color: 255, 0, 0", t_parentID);
    addElement<TextUIElement>("position: 0, 0, 2, 2; text: 3; character_size: 16; color: 255, 0, 0", t_parentID);
    addElement<TextUIElement>("position: 0, 0, 3, 3; text: 4; character_size: 16; color: 255, 0, 0", t_parentID);
    addElement<TextUIElement>("position: 0, 0, 4, 4; text: 5; character_size: 16; color: 255, 0, 0", t_parentID);
    addElement<TextUIElement>("position: 0, 0, 5, 5; text: 6; character_size: 16; color: 255, 0, 0", t_parentID);
    addElement<TextUIElement>("position: 0, 0, 6, 6; text: 7; character_size: 16; color: 255, 0, 0", t_parentID);
    addElement<TextUIElement>("position: 0, 0, 7, 7; text: 8; character_size: 16; color: 255, 0, 0", t_parentID);
    addElement<TextUIElement>("position: 0, 0, 8, 8; text: 9; character_size: 16; color: 255, 0, 0", t_parentID);

    addElement<ImageUIElement>("position: 0, 0, 6, 7; texture: images/dr bee.jpg; size: 100, 100, 0");

    updateVisuals();
}

unsigned int UILayer::getNewID()
{
    m_IDCounter++;

    return m_IDCounter - 1;
}

sf::Vector2f UILayer::getUIViewSize()
{
    return m_UIView.getSize();
}

void UILayer::deleteElement(unsigned int p_ID)
{
    if (m_elements.find(p_ID) == m_elements.end()) return;
    if (!m_elements[p_ID]) return;

    m_elements.erase(p_ID);
}

UIElement* UILayer::getElement(unsigned int p_ID)
{
    if (m_elements.find(p_ID) == m_elements.end()) return nullptr;
    
    return m_elements[p_ID].get();
}

UIManagementSystem* UILayer::getUIManagementSystem()
{
    return &m_uiManagementSystem;
}

void UILayer::updateVisuals()
{
    setUIViewSize();
    m_UIView.setCenter({m_UIView.getSize().x / 2.f, m_UIView.getSize().y / 2.f});
    
    for (auto& i_entry : m_elements)
    {
        i_entry.second->updateVisuals();
    }
}

void UILayer::update(float p_dt)
{
    for (auto& i_entry : m_elements)
    {
        i_entry.second->update(p_dt);
    }
}

void UILayer::tick()
{

}

void UILayer::draw(bool p_debug)
{
    std::vector<UIElement*> l_worldElements;
    std::vector<UIElement*> l_screenElements;

    for (auto& i_entry : m_elements)
    {
        if (!i_entry.second->m_visible) continue;

        if (i_entry.second->getUIPosition().m_worldPosition)
        {
            l_worldElements.emplace_back(i_entry.second.get());
        }
        else
        {
            l_screenElements.emplace_back(i_entry.second.get());
        }
    }

    std::sort(l_worldElements.begin(), l_worldElements.end(), [](UIElement* a, UIElement* b){
        return a->m_layer < b->m_layer;
    });

    std::sort(l_screenElements.begin(), l_screenElements.end(), [](UIElement* a, UIElement* b){
        return a->m_layer < b->m_layer;
    });

    for (auto i_worldElement : l_worldElements)
    {
        i_worldElement->draw(p_debug);

        if (!p_debug) continue;
        
        sf::RectangleShape rect(i_worldElement->getSize());
        rect.setPosition(i_worldElement->getGlobalBounds().position);
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Blue);
        rect.setOutlineThickness(.7f);
        m_game->getWindow()->draw(rect);

        sf::CircleShape l_circ(2);
        l_circ.setOrigin({2, 2});
        l_circ.setPosition(i_worldElement->getGlobalBounds().position);
        l_circ.setFillColor(sf::Color::Red);
        m_game->getWindow()->draw(l_circ);
    }

    m_game->getWindow()->setView(m_UIView);

    for (auto i_screenElement : l_screenElements)
    {
        i_screenElement->draw(p_debug);
        
        if (!p_debug) continue;
        
        sf::RectangleShape rect(i_screenElement->getSize());
        rect.setPosition(i_screenElement->getGlobalBounds().position);
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Blue);
        rect.setOutlineThickness(.7f);
        m_game->getWindow()->draw(rect);
        
        sf::CircleShape l_circ(2);
        l_circ.setOrigin({2, 2});
        l_circ.setPosition(i_screenElement->getGlobalBounds().position);
        l_circ.setFillColor(sf::Color::Red);
        m_game->getWindow()->draw(l_circ);
    }

    m_game->getWindow()->setView(m_camera->getView());
}

void UILayer::setUIViewSize()
{
    float l_baseSize = std::fmin(1080.f, m_game->getWindow()->getSize().y);
    
    m_UIView.setSize({l_baseSize * m_game->getWindow()->getAspectRatio(), l_baseSize});
}