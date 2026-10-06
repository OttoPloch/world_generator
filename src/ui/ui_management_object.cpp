#include "ui_management_object.hpp"
#include "../core/game.hpp"
#include "frame_ui_element.hpp"
#include "text_ui_element.hpp"
#include "ui_element.hpp"
#include "ui_layer.hpp"

UIManagementObject::UIManagementObject() {}

UIManagementObject::UIManagementObject(bool& pr_successful, Game* p_game, std::string p_objectType) : m_game(p_game), m_uiLayer(m_game->getScene()->getUILayer())
{
    std::vector<std::pair<std::string, std::string>> l_data = getDataFromFile(pr_successful, m_game, p_objectType);
    if (!pr_successful) return;
    
    setupFromData(pr_successful, p_objectType, l_data);
    if (!pr_successful) return;
}


void UIManagementObject::updateUI(std::unordered_map<std::string, std::string> p_variables)
{
    // go through the management data and replace the variable names with the values.

    std::cout << "variables received:\n";
    for (auto i : p_variables)
    {
        std::cout << "  - " << i.first << ": " << i.second << '\n';
    }
}

std::vector<std::pair<std::string, std::string>> UIManagementObject::getDataFromFile(bool& pr_successful, Game* p_game, std::string p_objectType)
{
    std::vector<std::pair<std::string, std::string>> l_variables;

    std::vector<std::string> l_fileData = p_game->getAssetManager()->getTextFromFile(p_objectType + ".ui", "content/ui/");

    for (auto i_line : l_fileData)
    {
        // specify if the line is for setup or update here.

        std::vector<std::string> l_segments = getSegmentsFromString(i_line, "- ");
        if (l_segments.size() != 2)
        {
            std::cerr << "ERROR: improper .ui file notation for object type: " << p_objectType << '\n';
            pr_successful = false;
            return {};
        }

        l_variables.emplace_back(l_segments[0], l_segments[1]);
    }

    pr_successful = true;
    return l_variables;
}

void UIManagementObject::setupFromData(bool& pr_successful, std::string p_objectType, std::vector<std::pair<std::string, std::string>> l_data)
{
    UIElement* l_parentElement = nullptr;
    std::vector<UIElement*> l_childElements;
    bool l_firstElementIsParent = false;

    for (auto& i_variable : l_data)
    {
        std::string l_name = i_variable.first;
        std::string l_value = i_variable.second;
    
        executeData(pr_successful, l_name, l_value, p_objectType, l_firstElementIsParent, l_parentElement, l_childElements);
        if (!pr_successful) return;
    }

    if (l_childElements.size() == 0)
    {
        pr_successful = true;
        return;
    }

    if (l_firstElementIsParent)
    {
        l_parentElement = l_childElements[0];
        l_childElements.erase(l_childElements.begin());
    }

    if (!l_parentElement)
    {
        std::cerr << "ERROR: no parent element created in setup for UI management object. UI management objects require one element to parent the rest.\n";
        pr_successful = false;
        return;
    }

    for (auto& i_child : l_childElements)
    {
        i_child->setParent(l_parentElement);
    }

    pr_successful = true;
    return;
}

void UIManagementObject::executeData(bool& pr_successful, std::string p_name, std::string p_value, std::string p_objectType, bool& pr_firstElementIsParent, UIElement* pr_parentElement, std::vector<UIElement*>& p_childElements)
{
    if (p_name == "type")
    {
        if (p_value != p_objectType)
        {
            std::cerr << "ERROR: type variable in .ui file for " << p_objectType << " does not match.\n";
            pr_successful = false;
            return;
        }
    }
    else if (p_name == "parent")
    {
        if (p_value == "first")
        {
            pr_firstElementIsParent = true;
            return;
        }

        UIElement* l_newElement = m_uiLayer->addElement(std::make_unique<UIElement>(m_game, p_value));
        pr_parentElement = l_newElement;
    }
    else if (p_name == "text")
    {
        UIElement* l_newTextElement = m_uiLayer->addElement(std::make_unique<TextUIElement>(m_game, p_value));
        p_childElements.emplace_back(l_newTextElement);
    }
    else if (p_name == "frame")
    {
        UIElement* l_newFrameElement = m_uiLayer->addElement(std::make_unique<FrameUIElement>(m_game, p_value));
        p_childElements.emplace_back(l_newFrameElement);
    }
}