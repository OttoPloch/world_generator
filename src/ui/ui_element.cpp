#include "ui_element.hpp"
#include "../core/game.hpp"
#include "ui_position.hpp"
#include <SFML/System/Vector2.hpp>
#include <algorithm>

UIElement::UIElement(Game* p_game, UIElement* p_parent) : m_game(p_game), m_window(p_game->getWindow()), m_parent(nullptr), m_identifier("none")
{
    if (p_parent) setParent(p_parent);
}

UIElement::UIElement(Game* p_game, std::string p_data, UIElement* p_parent) : m_game(p_game), m_window(p_game->getWindow()), m_parent(nullptr)
{
    if (p_parent) setParent(p_parent);
    setData(p_data);
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

std::string UIElement::getIdentifier()
{
    return m_identifier;
}

UIElement* UIElement::getChildByIdentifier(std::string p_childIdentifierPath)
{
    std::cout << "IDENTIFIER: " << p_childIdentifierPath << '\n';

    std::vector<std::string> l_identifierChain = getSegmentsFromString(p_childIdentifierPath, ".");
    if (l_identifierChain.size() == 0) return nullptr;

    if (l_identifierChain.size() == 1)
    {
        for (auto& i_child : m_children)
        {
            if (i_child->getIdentifier() == l_identifierChain[0])
            {
                std::cout << "ONLY 1, RETURNING " << i_child->getIdentifier() << '\n';

                return i_child;
            }
        }
    }
    else
    {
        std::string l_trimmedIdentifier = p_childIdentifierPath;
        l_trimmedIdentifier.erase(0, l_identifierChain[0].size() + 1);

        for (auto& i_child : m_children)
        {
            if (i_child->getIdentifier() == l_identifierChain[0])
            {
                std::cout << "REQUESTED MULTIPLE, ASKING CHILD FOR " << l_trimmedIdentifier << '\n';

                return i_child->getChildByIdentifier(l_trimmedIdentifier);
            }
        }
    }

    std::cout << "NO CHILD FOUND\n";
    return nullptr;
}

UIElement* UIElement::getParent()
{
    return m_parent;
}

void UIElement::addChild(UIElement* p_child)
{
    if (!p_child) return;
    if (std::find(m_children.begin(), m_children.end(), p_child) != m_children.end()) return;

    m_children.emplace_back(p_child);
}

void UIElement::removeChild(UIElement* p_child)
{
    if (!p_child) return;

    auto l_entry = std::find(m_children.begin(), m_children.end(), p_child);
    if (l_entry == m_children.end()) return;

    m_children.erase(l_entry);
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

void UIElement::setParent(UIElement* p_parent)
{
    if (m_parent)
    {
        m_parent->removeChild(this);
    }

    m_parent = p_parent;
    m_parent->addChild(this);
}

void UIElement::processDataCommand_Child(std::string p_key, std::string p_value)
{

}

void UIElement::updateVisuals_Child()
{

}

void UIElement::processDataCommand(std::string p_command)
{
    std::cout << "COMMAND: " << p_command << "\n";

    auto l_splitterIndex = p_command.find(": ");

    auto l_key = p_command.substr(0, l_splitterIndex);
    auto l_value = p_command.substr(l_splitterIndex + 2);

    if (l_key == "position")
    {
        setUIPositionFromString(l_value);
    }
    else if (l_key == "identifier")
    {
        m_identifier = l_value;
    }
    else
    {
        processDataCommand_Child(l_key, l_value);
    }
}

sf::Vector2f UIElement::calculateGlobalPosition()
{
    return m_position.m_anchorOffset + m_position.m_originOffset + m_position.m_offset;
}

void UIElement::setUIPositionFromString(std::string p_positionData)
{
    std::vector<std::string> l_values = getSegmentsFromString(p_positionData, ", ");
    if (l_values.size() < 2 || l_values.size() > 4) return;

    float l_x = 0, l_y = 0;
    UIOrigin l_origin = UIOrigin::TOP_LEFT;
    UIAnchor l_anchor = UIAnchor::TOP_LEFT;
    
    l_x = std::stof(l_values[0]);
    l_y = std::stof(l_values[1]);

    if (l_values.size() >= 3)
    {
        unsigned int l_originInt = std::stoul(l_values[2]);
        if (l_originInt >= enumSize<UIOrigin>())
        {
            std::cerr << "ERROR: origin provided for ui element position from a string is not one of the options. Full string is: " << p_positionData << '\n';
            return;
        }
        l_origin = static_cast<UIOrigin>(l_originInt);
    }

    if (l_values.size() >= 4)
    {
        unsigned int l_anchorInt = std::stoul(l_values[3]);
        if (l_anchorInt >= enumSize<UIAnchor>())
        {
            std::cerr << "ERROR: anchor provided for ui element position from a string is not one of the options. Full string is: " << p_positionData << '\n';
            return;
        }
        l_anchor = static_cast<UIAnchor>(l_anchorInt);
    }

    m_position = UIPosition({l_x, l_y}, l_origin, l_anchor);
}