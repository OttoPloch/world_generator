#include "ui_management_object.hpp"
#include "../core/game.hpp"
#include "frame_ui_element.hpp"
#include "text_ui_element.hpp"
#include "ui_element.hpp"
#include "ui_layer.hpp"
#include <unordered_map>

UIManagementObject::UIManagementObject() {}

UIManagementObject::UIManagementObject(bool& pr_successful, Game* p_game, std::string p_objectType) : m_game(p_game), m_uiLayer(m_game->getScene()->getUILayer())
{
    DataParser* l_dataParser = m_game->getAssetManager()->getDataParser();

    // std::unordered_map<std::string, UIElement*> l_namesToElements;
    // std::unordered_map<std::string, std::string> l_namesAndParentNames;

    std::unordered_map<std::string, DataObject> l_dataObjects;
    l_dataParser->loadData(pr_successful, l_dataObjects, "content/ui/" + p_objectType);
    if (l_dataObjects.find("setup") == l_dataObjects.end() || l_dataObjects.find("update") == l_dataObjects.end())
    {
        std::cerr << "ERROR: Either no setup block or no update block was found in data for UI management object of type '" << p_objectType << "'\n";
        pr_successful = false;
        return;
    }

    // DataObject& l_setupBlock = l_dataObjects["setup"];
    // processSetupBlock(pr_successful, l_setupBlock, l_namesAndParentNames, l_namesToElements);

    // DataObject& l_updateBlock = l_dataObjects["update"];
    // processUpdateBlock(pr_successful, l_updateBlock, l_namesToElements);

    // for (auto& i_entry : l_namesAndParentNames)
    // {
    //     std::string l_elementName = i_entry.first;
    //     std::string l_parentName = i_entry.second;
  
    //     if (l_parentName == ".") continue;

    //     auto l_elementEntry = l_namesToElements.find(l_elementName);
    //     if (l_elementEntry == l_namesToElements.end()) continue;
    //     if (!l_elementEntry->second) continue;

    //     auto l_parentEntry = l_namesToElements.find(l_parentName);
    //     if (l_parentEntry == l_namesToElements.end()) continue;
    //     if (!l_parentEntry->second) continue;

    //     UIElement* l_element = l_elementEntry->second;
    //     UIElement* l_parent = l_parentEntry->second;

    //     l_element->setParent(l_parent);
    // }
}

void UIManagementObject::updateUI(std::unordered_map<std::string, std::string> p_variables)
{
    // for (auto& i_updateObject : m_updateData)
    // {
    //     std::string l_preparedUpdateData;
    //     prepareUpdateData(l_preparedUpdateData, i_updateObject.updateData, p_variables);

    //     if (!i_updateObject.element) continue;

    //     i_updateObject.element->setData(l_preparedUpdateData);
    // }
}

// void UIManagementObject::createElementFromData(bool& pr_successful, SetupObject p_setupObject, std::unordered_map<std::string, UIElement*>& p_namesToElements)
// {
//     UIElement* l_newElement;

//     if (p_setupObject.elementType == "base")
//     {
//         l_newElement = m_uiLayer->addElement<UIElement>(p_setupObject.setupData);
//     }
//     else if (p_setupObject.elementType == "text")
//     {
//         l_newElement = m_uiLayer->addElement<TextUIElement>(p_setupObject.setupData);
//     }
//     else if (p_setupObject.elementType == "frame")
//     {
//         l_newElement = m_uiLayer->addElement<FrameUIElement>(p_setupObject.setupData);
//     }

//     p_namesToElements[p_setupObject.elementName] = l_newElement;
// }

// void UIManagementObject::processSetupBlock(bool& pr_successful, DataBlock& p_setupBlock, std::unordered_map<std::string, std::string>& p_namesAndParentNames, std::unordered_map<std::string, UIElement*>& p_namesToElements)
// {
//     for (auto& i_mapEntry : p_setupBlock.entries)
//     {
//         DataEntry& l_blockEntry = i_mapEntry.second;
        
//         std::unordered_map<std::string, std::string>& l_variables = l_blockEntry.variables;
//         if (l_variables.find("parent") == l_variables.end() || l_variables.find("type") == l_variables.end() || l_variables.find("data") == l_variables.end())
//         {
//             std::cerr << "ERROR: one or more variables missing from entry in setup block of ui data file. Entry is '" << i_mapEntry.first << "'\n";
//             pr_successful = false;
//             return;
//         }
        
//         std::string l_elementName = i_mapEntry.first;
//         std::string l_parentName = l_variables["parent"];
//         std::string l_type = l_variables["type"];
//         std::string l_setupData = l_variables["data"];
        
//         p_namesAndParentNames[l_elementName] = l_parentName;
        
//         createElementFromData(pr_successful, {l_elementName, l_parentName, l_type, l_setupData}, p_namesToElements);
//     }
// }

// void UIManagementObject::processUpdateBlock(bool& pr_successful, DataBlock& p_updateBlock, std::unordered_map<std::string, UIElement*>& p_namesToElements)
// {
//     for (auto& i_mapEntry : p_updateBlock.entries)
//     {
//         DataEntry& l_blockEntry = i_mapEntry.second;
        
//         std::unordered_map<std::string, std::string>& l_variables = l_blockEntry.variables;
//         if (l_variables.find("data") == l_variables.end())
//         {
//             std::cerr << "ERROR: data variable missing from entry in update block of ui data file. Entry is '" << i_mapEntry.first << "'\n";
//             pr_successful = false;
//             return;
//         }
        
//         std::string l_elementName = i_mapEntry.first;
//         std::string l_updateData = l_variables["data"];
        
//         auto l_entry = p_namesToElements.find(l_elementName);
//         if (l_entry != p_namesToElements.end())
//         {
//             m_updateData.push_back({l_entry->second, l_updateData});
//         }
//     }
// }

// void UIManagementObject::prepareUpdateData(std::string& pr_preparedUpdateData, std::string p_updateData, const std::unordered_map<std::string, std::string>& p_variables)
// {
//     pr_preparedUpdateData = p_updateData;

//     auto l_opening = pr_preparedUpdateData.find("{");
//     auto l_closing = pr_preparedUpdateData.find("}");
//     while (l_opening != std::string::npos && l_closing != std::string::npos)
//     {
//         int l_diff = l_closing - l_opening;
//         std::string l_variableName = pr_preparedUpdateData.substr(l_opening + 1, l_diff - 1);
//         auto l_entry = p_variables.find(l_variableName);
        
//         std::string l_value;
//         if (l_entry != p_variables.end())
//         {
//             l_value = l_entry->second;
//         }
//         else
//         {
//             l_value = "ERR VAR NAME NOT FOUND";
//         }
        
//         pr_preparedUpdateData.erase(l_opening, l_diff + 1);
//         pr_preparedUpdateData.insert(l_opening, l_value);

//         l_opening = pr_preparedUpdateData.find("{");
//         l_closing = pr_preparedUpdateData.find("}");
//     }
// }