#include "ui_management_system.hpp"
#include "ui_management_object.hpp"
#include <iostream>
#include <limits>

UIManagementSystem::UIManagementSystem(Game* p_game) : m_game(p_game), m_IDCounter(0) {}

unsigned int UIManagementSystem::addObject(bool& pr_successful, std::string p_objectType)
{
    unsigned int l_newID = getNewID();

    m_objects[l_newID] = std::make_unique<UIManagementObject>(pr_successful, m_game, p_objectType);
    if (!pr_successful)
    {
        m_objects.erase(l_newID);
        return std::numeric_limits<unsigned int>().max();
    }

    return l_newID;
}

void UIManagementSystem::removeObject(unsigned int p_ID)
{
    if (m_objects.find(p_ID) == m_objects.end()) return;
    if (!m_objects[p_ID]) return;

    m_objects.erase(p_ID);
}

void UIManagementSystem::updateObject(unsigned int p_ID, std::unordered_map<std::string, std::string> p_variables)
{
    if (m_objects.find(p_ID) == m_objects.end()) return;
    if (p_ID == std::numeric_limits<unsigned int>().max()) return;

    m_objects[p_ID]->updateUI(p_variables);
}

unsigned int UIManagementSystem::getNewID()
{
    m_IDCounter++;

    return m_IDCounter - 1;
}