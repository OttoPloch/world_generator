#include "ui_management_object.hpp"
#include "../core/game.hpp"
#include "frame_ui_element.hpp"
#include "text_ui_element.hpp"
#include "ui_element.hpp"
#include "ui_layer.hpp"

SetupObject::SetupObject(std::string p_elementName, std::string p_elementType, std::string p_setupData) : m_elementName(p_elementName), m_elementType(p_elementType), m_setupData(p_setupData) {}
UpdateObject::UpdateObject(UIElement* p_element, std::string p_updateData) : m_element(p_element), m_updateData(p_updateData) {}

UIManagementObject::UIManagementObject() {}

UIManagementObject::UIManagementObject(bool& pr_successful, Game* p_game, std::string p_objectType) : m_game(p_game), m_uiLayer(m_game->getScene()->getUILayer())
{
    std::vector<SetupObject> l_setupData;
    std::unordered_map<std::string, std::string> l_namesToUpdateData;

    getDataFromFile(pr_successful, m_game, p_objectType, l_setupData, l_namesToUpdateData);
    if (!pr_successful) return;
    
    setupFromData(pr_successful, p_objectType, l_setupData, l_namesToUpdateData);
    if (!pr_successful) return;
}

void UIManagementObject::updateUI(std::unordered_map<std::string, std::string> p_variables)
{
    for (auto& i_updateObject : m_updateData)
    {
        std::string l_preparedUpdateData;
        prepareUpdateData(l_preparedUpdateData, i_updateObject.m_updateData, p_variables);

        if (!i_updateObject.m_element) continue;

        i_updateObject.m_element->setData(l_preparedUpdateData);
    }
}

void UIManagementObject::getDataFromFile(bool& pr_successful, Game* p_game, std::string p_objectType, std::vector<SetupObject>& pr_setupData, std::unordered_map<std::string, std::string>& pr_namesToUpdateData)
{
    std::vector<std::string> l_fileData = p_game->getAssetManager()->getTextFromFile(p_objectType + ".ui", "content/ui/");

    for (auto i_line : l_fileData)
    {
        std::vector<std::string> l_segments;

        if (i_line.substr(0, 6) == "type- ")
        {
            if (i_line.substr(6) != p_objectType)
            {
                std::cerr << "ERROR: type variable in .ui file for " << p_objectType << " does not match. It says: " << i_line.substr(6) << '\n';
                pr_successful = false;
                return;
            }
        }
        else if (i_line.substr(0, 6) == "setup ")
        {
            l_segments = getSegmentsFromString(i_line.substr(6), "- ");

            if (l_segments.size() != 3)
            {
                std::cerr << "ERROR: improper .ui file notation for setup for object type: " << p_objectType << '\n';
                pr_successful = false;
                return;
            }

            pr_setupData.emplace_back(l_segments[0], l_segments[1], l_segments[2]);
        }
        else if (i_line.substr(0, 7) == "update ")
        {
            l_segments = getSegmentsFromString(i_line.substr(7), "- ");

            if (l_segments.size() != 2)
            {
                std::cerr << "ERROR: improper .ui file notation for update for object type: " << p_objectType << '\n';
                pr_successful = false;
                return;
            }

            pr_namesToUpdateData[l_segments[0]] = l_segments[1];
        }
        else
        {
            continue;
        }
    }

    pr_successful = true;
}

void UIManagementObject::setupFromData(bool& pr_successful, std::string p_objectType, const std::vector<SetupObject>& p_setupData, std::unordered_map<std::string, std::string>& p_namesToUpdateData)
{
    std::vector<UIElement*> l_childElements;

    for (auto& i_setupObject : p_setupData)
    {
        executeSetupData(pr_successful, i_setupObject, p_objectType, l_childElements, p_namesToUpdateData);
        if (!pr_successful) return;
    }

    if (l_childElements.size() == 0)
    {
        pr_successful = true;
        return;
    }

    if (!m_parentElement)
    {
        std::cerr << "ERROR: no parent element created in setup for UI management object. UI management objects require one element to parent the rest.\n";
        pr_successful = false;
        return;
    }

    // replace with complex family support
    for (auto& i_child : l_childElements)
    {
        i_child->setParent(m_parentElement);
    }

    pr_successful = true;
    return;
}

void UIManagementObject::executeSetupData(bool& pr_successful, SetupObject p_setupObject, std::string p_objectType, std::vector<UIElement*>& pr_childElements, std::unordered_map<std::string, std::string>& p_namesToUpdateData)
{
    UIElement* l_newElement;

    if (p_setupObject.m_elementType == "base")
    {
        l_newElement = m_uiLayer->addElement<UIElement>(p_setupObject.m_setupData);
    }
    else if (p_setupObject.m_elementType == "text")
    {
        l_newElement = m_uiLayer->addElement<TextUIElement>(p_setupObject.m_setupData);
    }
    else if (p_setupObject.m_elementType == "frame")
    {
        l_newElement = m_uiLayer->addElement<FrameUIElement>(p_setupObject.m_setupData);
    }

    if (p_setupObject.m_elementName == "parent")
    {
        m_parentElement = l_newElement;
    }
    else
    {
        pr_childElements.emplace_back(l_newElement);
    }

    auto l_entry = p_namesToUpdateData.find(p_setupObject.m_elementName);
    if (l_entry != p_namesToUpdateData.end())
    {
        m_updateData.emplace_back(l_newElement, l_entry->second);
    }
}

void UIManagementObject::prepareUpdateData(std::string& pr_preparedUpdateData, std::string p_updateData, const std::unordered_map<std::string, std::string>& p_variables)
{
    pr_preparedUpdateData = p_updateData;

    auto l_opening = pr_preparedUpdateData.find("{");
    auto l_closing = pr_preparedUpdateData.find("}");
    while (l_opening != std::string::npos && l_closing != std::string::npos)
    {
        int l_diff = l_closing - l_opening;
        std::string l_variableName = pr_preparedUpdateData.substr(l_opening + 1, l_diff - 1);
        auto l_entry = p_variables.find(l_variableName);
        
        std::string l_value;
        if (l_entry != p_variables.end())
        {
            l_value = l_entry->second;
        }
        else
        {
            l_value = "ERR VAR NAME NOT FOUND";
        }
        
        pr_preparedUpdateData.erase(l_opening, l_diff + 1);
        pr_preparedUpdateData.insert(l_opening, l_value);

        l_opening = pr_preparedUpdateData.find("{");
        l_closing = pr_preparedUpdateData.find("}");
    }
}