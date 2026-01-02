#pragma once

#include "common.hpp"

#include <map>

class TileSet
{
public:
    TileSet();

    TileSet(std::string setName, sf::Texture* texture, std::unordered_map<std::string, sf::Vector2f> texCoords, float tileSize);

    void init(std::string setName, sf::Texture* texture, std::unordered_map<std::string, sf::Vector2f> texCoords, float tileSize);

    sf::Vector2f getCoordsFor(std::string key);

    std::string getKeyFor(sf::Vector2f texCoord);

    sf::Texture* getTexture();

    float getTileSize();
private:
    std::string setName;

    sf::Texture* texture;

    std::unordered_map<std::string, sf::Vector2f> texCoords;

    float tileSize;
};