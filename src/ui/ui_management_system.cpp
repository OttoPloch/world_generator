#include "ui_management_system.hpp"
#include <limits>

UIManagementSystem::UIManagementSystem(Game* p_game) : m_game(p_game) {}

unsigned int UIManagementSystem::addObject(bool& pr_successful, std::string p_objectType)
{
    unsigned int l_newID = getNewID();

    auto l_newObject = UIManagementObject(pr_successful, m_game, p_objectType);
    if (!pr_successful) return 0;

    m_objects[l_newID] = l_newObject;

    return l_newID;
}

void UIManagementSystem::removeObject(unsigned int p_ID)
{
    auto l_entry = m_objects.find(p_ID);

    if (l_entry == m_objects.end()) return;
    
    m_objects.erase(l_entry);
}

void UIManagementSystem::updateObject(unsigned int p_ID, std::unordered_map<std::string, std::string> p_variables)
{
    if (m_objects.find(p_ID) == m_objects.end()) return;

    m_objects[p_ID].updateUI(p_variables);
}

unsigned int UIManagementSystem::getNewID()
{
    m_IDCounter++;

    return m_IDCounter - 1;
}