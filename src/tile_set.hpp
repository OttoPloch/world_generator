#pragma once

#include "common.hpp"

#include <map>

class TileSet
{
public:
    TileSet();

    TileSet(std::string setName, std::unordered_map<std::string, sf::Vector2f> texCoords, float tileSize);

    void init(std::string setName, std::unordered_map<std::string, sf::Vector2f> texCoords, float tileSize);

    sf::Vector2f getCoordsFor(std::string key);

    std::string getKeyFor(sf::Vector2f texCoord);

    bool hasCenter();
    
    std::string setName;
    std::unordered_map<std::string, sf::Vector2f> texCoords;
    float tileSize;
};