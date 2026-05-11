#pragma once

#include <unordered_map>

#include "tile_template.hpp"

class TileTemplateManager
{
public:
    TileTemplateManager();

    std::unordered_map<std::string, TileTemplate> tileTemplates;
};