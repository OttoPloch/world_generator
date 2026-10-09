#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class Game;
class UIElement;
class UILayer;

struct SetupObject
{
    SetupObject(std::string p_elementName, std::string p_elementType, std::string p_setupData);

    std::string m_elementName;
    std::string m_elementType;
    std::string m_setupData;
};

struct UpdateObject
{
    UpdateObject(UIElement* p_element, std::string p_updateData);

    UIElement* m_element;
    std::string m_updateData;
};

class UIManagementObject
{
public:
    UIManagementObject();

    UIManagementObject(bool& pr_successful, Game* p_game, std::string p_objectType);

    void updateUI(std::unordered_map<std::string, std::string> p_variables);
private:
    void getDataFromFile(bool& pr_successful, Game* p_game, std::string p_objectType, std::vector<SetupObject>& pr_setupData, std::unordered_map<std::string, std::string>& pr_namesoToUpdateData);

    void setupFromData(bool& pr_successful, std::string p_objectType, const std::vector<SetupObject>& p_setupData, std::unordered_map<std::string, std::string>& p_namesToUpdateData);

    void executeSetupData(bool& pr_successful, SetupObject p_setupObject, std::string p_objectType, std::vector<UIElement*>& pr_childElements, std::unordered_map<std::string, std::string>& p_namesToUpdateData);

    void prepareUpdateData(std::string& pr_preparedUpdateData, std::string p_updateData, const std::unordered_map<std::string, std::string>& p_variables);

    Game* m_game;
    UILayer* m_uiLayer;

    std::vector<UpdateObject> m_updateData;
    UIElement* m_parentElement;
};