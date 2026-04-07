#pragma once

#include <unordered_map>

#include "entity_template.hpp"

class TemplateManager
{
public:
    TemplateManager();

    std::unordered_map<std::string, EntityTemplate> entityTemplates;
};