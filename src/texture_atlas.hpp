#pragma once

#include "common.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <unordered_map>

class TextureAtlas
{
public:
    TextureAtlas();

    TextureAtlas(std::string atlasName, std::unordered_map<std::string, sf::FloatRect> itemTexCoords);

    sf::FloatRect getItemTexCoords(std::string item);

    std::string name;
    std::unordered_map<std::string, sf::FloatRect> itemTexCoords;
};