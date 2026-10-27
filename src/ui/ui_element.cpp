#include "ui_element.hpp"
#include "../core/game.hpp"
#include "ui_position.hpp"
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cstdint>
#include <limits>

UIElement::UIElement(Game* p_game, unsigned int p_ID, unsigned int p_parent) : m_game(p_game), m_window(p_game->getWindow()), m_ID(p_ID), m_parent(UINT32_MAX), m_visible(true)
{
    setParent(p_parent);
}

UIElement::UIElement(Game* p_game, unsigned int p_ID, std::string p_data, unsigned int p_parent) : m_game(p_game), m_window(p_game->getWindow()), m_ID(p_ID), m_parent(UINT32_MAX), m_visible(true)
{
    setParent(p_parent);
    setData(p_data);
}

UIElement::~UIElement()
{
    for (auto i_childID : m_children)
    {
        UIElement* l_childElement = m_game->getScene()->getUILayer()->getElement(i_childID);

        if (l_childElement)
        {
            l_childElement->setParent();
        }
    }

    if (m_parent)
    {
        UIElement* l_parentElement = m_game->getScene()->getUILayer()->getElement(m_parent);

        if (l_parentElement)
        {
            l_parentElement->removeChild(m_ID);
        }
    }
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

    for (auto& i_entry : m_children)
    {
        UIElement* l_childElement = m_game->getScene()->getUILayer()->getElement(i_entry);
        if (!l_childElement) continue;

        l_childElement->updateVisuals();
    }
}

UIPosition UIElement::getUIPosition()
{
    return m_position;
}

sf::FloatRect UIElement::getGlobalBounds()
{
    return {m_globalPosition, getSize()};
}

unsigned int UIElement::getID()
{
    return m_ID;
}

// UIElement* UIElement::getChildByIDPath(std::string p_childIDPath)
// {
//     std::vector<unsigned int> l_identifierChain = getUIntsFromString(p_childIDPath, ".");
//     if (l_identifierChain.size() == 0) return nullptr;

//     if (l_identifierChain.size() == 1)
//     {
//         for (auto& i_entry : m_children)
//         {
//             if (i_entry.second->getID() == l_identifierChain[0])
//             {
//                 return i_entry.second;
//             }
//         }
//     }
//     else
//     {
//         std::string l_trimmedIDPath = p_childIDPath;
//         l_trimmedIDPath.erase(0, std::to_string(l_identifierChain[0]).size() + 1);

//         for (auto& i_entry : m_children)
//         {
//             if (i_entry.second->getID() == l_identifierChain[0])
//             {
//                 return i_entry.second->getChildByIDPath(l_trimmedIDPath);
//             }
//         }
//     }

//     return nullptr;
// }

void UIElement::setParent(unsigned int p_parent)
{
    if (m_parent != UINT32_MAX)
    {
        UIElement* l_parent = m_game->getScene()->getUILayer()->getElement(m_parent);

        if (l_parent) l_parent->removeChild(m_ID);
    }

    m_parent = p_parent;
}

unsigned int UIElement::getParentID()
{
    return m_parent;
}

void UIElement::addChild(unsigned int p_childID)
{
    if (!m_game->getScene()->getUILayer()->getElement(p_childID)) return;

    m_children.emplace(p_childID);
}

void UIElement::removeChild(unsigned int p_childID)
{
    auto l_entry = m_children.find(p_childID);
    if (l_entry == m_children.end()) return;

    m_children.erase(l_entry);
}

sf::Vector2f UIElement::getSize()
{
    return {0, 0};
}

void UIElement::update(float p_dt)
{

}

void UIElement::draw(bool p_debug)
{

}

void UIElement::processDataCommand_Child(std::string p_key, std::string p_value)
{

}

void UIElement::updateVisuals_Child()
{

}

void UIElement::processDataCommand(std::string p_command)
{
    auto l_splitterIndex = p_command.find(": ");

    auto l_key = p_command.substr(0, l_splitterIndex);
    auto l_value = p_command.substr(l_splitterIndex + 2);

    if (l_key == "position")
    {
        setUIPositionFromString(l_value);
    }
    else
    {
        processDataCommand_Child(l_key, l_value);
    }

    updateVisuals();
}

sf::Vector2f UIElement::calculateGlobalPosition()
{
    return m_position.m_anchorOffset + m_position.m_originOffset + m_position.m_offset;
}

void UIElement::setUIPositionFromString(std::string p_positionData)
{
    std::vector<std::string> l_values = getSegmentsFromString(p_positionData, ", ");
    if (l_values.size() < 2 || l_values.size() > 5) return;

    float l_x = 0, l_y = 0;
    UIOrigin l_origin = m_position.m_origin;
    UIAnchor l_anchor = m_position.m_anchor;
    bool l_worldPosition = false;
    
    l_x = std::stof(l_values[0]);
    l_y = std::stof(l_values[1]);

    if (l_values.size() >= 3)
    {
        unsigned int l_originInt = std::stoul(l_values[2]);
        if (l_originInt >= enumSize<UIOrigin>())
        {
            std::cerr << "ERROR: Origin provided for ui element position from a string is not one of the options. Full string is: " << p_positionData << '\n';
            return;
        }
        l_origin = static_cast<UIOrigin>(l_originInt);
    }

    if (l_values.size() >= 4)
    {
        unsigned int l_anchorInt = std::stoul(l_values[3]);
        if (l_anchorInt >= enumSize<UIAnchor>())
        {
            std::cerr << "ERROR: Anchor provided for ui element position from a string is not one of the options. Full string is: " << p_positionData << '\n';
            return;
        }
        l_anchor = static_cast<UIAnchor>(l_anchorInt);
    }

    if (l_values.size() == 5)
    {
        l_worldPosition = std::stoi(l_values[4]);
    }

    m_position = UIPosition({l_x, l_y}, l_origin, l_anchor, l_worldPosition);
}