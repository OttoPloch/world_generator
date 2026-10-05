#pragma once

#include <string>

class UIElement;

class UIManagementObject
{
public:
    UIManagementObject();

    UIManagementObject(std::string p_objectType);

    UIManagementObject(const UIManagementObject& other);

    void updateUI(std::string p_data);

private:
    std::string m_managementData;
    UIElement* m_parentElement;
};