#pragma once

#include "common.hpp"

#include <map>

class TileSet
{
public:
    TileSet();

    TileSet(std::string setName, sf::Texture* texture, std::unordered_map<std::string, sf::Vector2f> texCoords);

    void init(std::string setName, sf::Texture* texture, std::unordered_map<std::string, sf::Vector2f> texCoords);

    sf::Vector2f getCoordsFor(std::string key);

    std::string getKeyFor(sf::Vector2f texCoord);

    sf::Texture* getTexture();
private:
    std::string setName;

    sf::Texture* texture;

    std::unordered_map<std::string, sf::Vector2f> texCoords;
};