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
    UpdateObject(std::string p_elementPath, std::string p_updateData);

    std::string m_elementPath;
    std::string m_updateData;
};

class UIManagementObject
{
public:
    UIManagementObject();

    UIManagementObject(bool& pr_successful, Game* p_game, std::string p_objectType);

    void updateUI(std::unordered_map<std::string, std::string> p_variables);
private:
    void getDataFromFile(bool& pr_successful, Game* p_game, std::string p_objectType, std::vector<SetupObject>& pr_setupData);

    void setupFromData(bool& pr_successful, std::string p_objectType, const std::vector<SetupObject>& p_setupData);

    void executeSetupData(bool& pr_successful, SetupObject p_setupObject, std::string p_objectType, std::vector<UIElement*>& pr_childElements, std::unordered_map<std::string, unsigned int>& pr_dataNamesToElementIDs);

    void prepareUpdateData(std::string& pr_preparedUpdateData, std::string pr_updateData, const std::unordered_map<std::string, std::string>& p_variables);

    void replaceNamesWithIDs(bool& pr_successful, const std::unordered_map<std::string, unsigned int>& p_dataNamesToElementIDs);

    Game* m_game;
    UILayer* m_uiLayer;

    std::vector<UpdateObject> m_updateData;
    UIElement* m_parentElement;
};