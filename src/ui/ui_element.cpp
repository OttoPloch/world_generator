#include "ui_element.hpp"
#include "../core/game.hpp"
#include "ui_position.hpp"
#include <SFML/Graphics/Rect.hpp>
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
    std::vector<UIElement*> l_childrenToEmancipate;
    for (auto i_childID : m_children)
    {
        UIElement* l_childElement = m_game->getScene()->getUILayer()->getElement(i_childID);

        if (l_childElement)
        {
            l_childrenToEmancipate.emplace_back(l_childElement);
        }
    }

    for (auto i_childElement : l_childrenToEmancipate)
    {
        i_childElement->setParent();
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

std::string UIElement::retrieveData(std::string p_dataToRetrieve)
{
    if (p_dataToRetrieve == "local_position")
    {
        return std::to_string(m_position.m_offset.x) + ", " + std::to_string(m_position.m_offset.y);
    }
    else if (p_dataToRetrieve == "global_position")
    {
        return std::to_string(m_globalPosition.x) + ", " + std::to_string(m_globalPosition.y);
    }
    else if (p_dataToRetrieve == "size")
    {
        sf::Vector2f l_size = getSize();
        return std::to_string(l_size.x) + ", " + std::to_string(l_size.y);
    }
    else if (p_dataToRetrieve == "bounding_box_position")
    {
        sf::Vector2f l_bbPosition = getFamilyBoundingBox().position;
        return std::to_string(l_bbPosition.x) + ", " + std::to_string(l_bbPosition.y);
    }
    else if (p_dataToRetrieve == "bounding_box_size")
    {
        sf::Vector2f l_bbSize = getFamilyBoundingBox().size;

        std::cerr << "senhofgiehfnoseif " << l_bbSize.x << ", " << l_bbSize.y << '\n';

        return std::to_string(l_bbSize.x) + ", " + std::to_string(l_bbSize.y);
    }
    else
    {
        return retrieveData_Child(p_dataToRetrieve);
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
        UIElement* l_parentElement = m_game->getScene()->getUILayer()->getElement(m_parent);

        if (l_parentElement) l_parentElement->removeChild(m_ID);
    }

    m_parent = p_parent;
    if (m_parent == UINT32_MAX) return;

    UIElement* l_parentElement = m_game->getScene()->getUILayer()->getElement(m_parent);
    if (!l_parentElement)
    {
        std::cerr << "ERROR: setting parent of UI element to ID '" << p_parent << "' failed, no element was found with that ID.\n";
        m_parent = UINT32_MAX;
        return;
    }

    l_parentElement->addChild(m_ID);
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

sf::FloatRect UIElement::getFamilyBoundingBox()
{
    sf::FloatRect l_globalBounds = getGlobalBounds();

    float left = l_globalBounds.position.x;
    float right = l_globalBounds.position.x + l_globalBounds.size.x;
    float top = l_globalBounds.position.y;
    float bottom = l_globalBounds.position.y + l_globalBounds.size.y;

    for (auto i_childID : m_children)
    {
        UIElement* l_childElement = m_game->getScene()->getUILayer()->getElement(i_childID);
        if (!l_childElement) continue;

        sf::FloatRect l_childBounds = l_childElement->getFamilyBoundingBox();

        left = std::min(left, l_childBounds.position.x);
        right = std::max(right, l_childBounds.position.x + l_childBounds.size.x);
        top = std::min(top, l_childBounds.position.y);
        bottom = std::max(bottom, l_childBounds.position.y + l_childBounds.size.y);

        std::cerr << bottom << "; " << l_childBounds.position.y << " + " << l_childBounds.size.y << '\n';
    }

    return {{left, top}, {right - left, bottom - top}};
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

std::string UIElement::retrieveData_Child(std::string p_dataToRetrieve)
{
    return "ERR NO CHILD OVERRIDE";
}

void UIElement::processDataCommand(std::string p_command)
{
    auto l_splitterIndex = p_command.find(": ");

    std::string l_key = p_command.substr(0, l_splitterIndex);
    std::string l_value = p_command.substr(l_splitterIndex + 2);

    if (l_key == "position")
    {
        setUIPositionFromString(l_value);
    }
    else if (l_key == "print")
    {
        std::cout << "UI element with ID " << m_ID << " says:\n   ";
        std::cout << l_value << '\n';
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