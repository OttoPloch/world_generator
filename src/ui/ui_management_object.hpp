#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "../utils/data_parser.hpp"

class Game;
class UIElement;
class UILayer;

struct SetupObject
{
    std::string elementName;
    std::string elementParent;
    std::string elementType;
    std::string setupData;
};

struct UpdateObject
{
    unsigned int elementID;
    std::string updateData;
};

class UIManagementObject
{
public:
    UIManagementObject();

    UIManagementObject(bool& pr_successful, Game* p_game, std::string p_objectType);

    ~UIManagementObject();

    void updateUI(std::unordered_map<std::string, std::string> p_variables);
private:
    void createElementFromData(bool& pr_successful, SetupObject p_setupObject, std::unordered_map<std::string, unsigned int>& p_namesToElementIDs);

    void processSetupBlock(bool& pr_successful, DataBlock& p_setupBlock, std::unordered_map<std::string, std::string>& p_namesAndParentNames, std::unordered_map<std::string, unsigned int>& p_namesToElementIDs);
    
    void processUpdateBlock(bool& pr_successful, DataBlock& p_updateBlock, std::unordered_map<std::string, unsigned int>& p_namesToElementIDs);
    
    void prepareUpdateData(std::string& pr_preparedUpdateData, std::string p_updateData, const std::unordered_map<std::string, std::string>& p_variables);

    Game* m_game;
    UILayer* m_uiLayer;

    std::vector<UpdateObject> m_updateData;
};