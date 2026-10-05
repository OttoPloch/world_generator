#include "ui_management_system.hpp"

UIManagementSystem::UIManagementSystem() {}

unsigned int UIManagementSystem::addObject(std::string p_objectType)
{
    unsigned int l_newID = getNewID();

    m_objects[l_newID] = UIManagementObject(p_objectType);

    return l_newID;
}

void UIManagementSystem::removeObject(unsigned int p_ID)
{
    auto l_entry = m_objects.find(p_ID);

    if (l_entry == m_objects.end()) return;
    
    m_objects.erase(l_entry);
}

void UIManagementSystem::updateObject(unsigned int p_ID, std::string p_data)
{
    if (m_objects.find(p_ID) == m_objects.end()) return;

    m_objects[p_ID].updateUI(p_data);
}

unsigned int UIManagementSystem::getNewID()
{
    m_IDCounter++;

    return m_IDCounter - 1;
}