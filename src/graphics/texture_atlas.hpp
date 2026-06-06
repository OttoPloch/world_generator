#pragma once

#include "../core/common.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <unordered_map>

class TextureAtlas
{
public:
    TextureAtlas();

    TextureAtlas(std::string atlasName, std::unordered_map<std::string, sf::FloatRect> itemTexCoords, unsigned int tileSize);

    std::string name;
    std::unordered_map<std::string, sf::FloatRect> itemTexCoords;
    unsigned int tileSize;
};