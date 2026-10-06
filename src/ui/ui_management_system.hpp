#pragma once

#include "ui_management_object.hpp"
#include <map>
#include <string>
#include <unordered_map>

class Game;
class UIElement;

class UIManagementSystem
{
public:
    UIManagementSystem(Game* p_game);

    // returns the ID of the new management object
    unsigned int addObject(bool& pr_successful, std::string p_objectType);

    void removeObject(unsigned int p_ID);

    void updateObject(unsigned int p_ID, std::unordered_map<std::string, std::string> p_variables);
private:
    unsigned int getNewID();

    unsigned int m_IDCounter;

    Game* m_game;

    std::map<unsigned int, UIManagementObject> m_objects;
};