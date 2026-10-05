#pragma once

#include "ui_management_object.hpp"
#include <map>
#include <string>

class UIElement;

class UIManagementSystem
{
public:
    UIManagementSystem();

    // returns the ID of the new management object
    unsigned int addObject(std::string p_objectType);

    void removeObject(unsigned int p_ID);

    void updateObject(unsigned int p_ID, std::string p_data);
private:
    unsigned int getNewID();

    unsigned int m_IDCounter;

    std::map<unsigned int, UIManagementObject> m_objects;
};