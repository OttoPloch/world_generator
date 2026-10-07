#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class Game;
class UIElement;
class UILayer;

class UIManagementObject
{
public:
    UIManagementObject();

    UIManagementObject(bool& pr_successful, Game* p_game, std::string p_objectType);

    void updateUI(std::unordered_map<std::string, std::string> p_variables);
private:
    std::vector<std::pair<std::string, std::string>> getDataFromFile(bool& pr_successful, Game* p_game, std::string p_objectType);

    void setupFromData(bool& pr_successful, std::string p_objectType, std::vector<std::pair<std::string, std::string>> l_data);

    void executeSetupData(bool& pr_successful, std::string p_name, std::string p_value, std::string p_objectType, bool& pr_firstElementIsParent, UIElement* pr_parentElement, std::vector<UIElement*>& p_childElements);

    void prepareUpdateData(std::string& pr_updateData, std::unordered_map<std::string, std::string>& p_variables);

    Game* m_game;
    UILayer* m_uiLayer;

    std::vector<std::pair<std::string, std::string>> m_updateData;
    UIElement* m_parentElement;
};