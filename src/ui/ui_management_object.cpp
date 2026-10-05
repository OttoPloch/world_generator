#include "ui_management_object.hpp"

UIManagementObject::UIManagementObject() {}

UIManagementObject::UIManagementObject(std::string p_objectType)
{
    
}

UIManagementObject::UIManagementObject(const UIManagementObject& other) : m_managementData(other.m_managementData), m_parentElement(other.m_parentElement)
{

}

void UIManagementObject::updateUI(std::string p_data)
{

}
