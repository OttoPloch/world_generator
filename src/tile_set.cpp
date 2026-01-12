#include "tile_set.hpp"

TileSet::TileSet() {}

TileSet::TileSet(std::string setName, std::unordered_map<std::string, sf::Vector2f> texCoords, float tileSize)
{
    init(setName, texCoords, tileSize);
}

void TileSet::init(std::string setName, std::unordered_map<std::string, sf::Vector2f> texCoords, float tileSize)
{
    this->setName = setName;

    this->texCoords = texCoords;

    this->tileSize = tileSize;
}

sf::Vector2f TileSet::getCoordsFor(std::string key)
{
    if (texCoords.find(key) != texCoords.end())
    {
        return texCoords[key];
    }
    else
    {
        std::cout << "ERROR loading texture coords for key of " << key << " in tile set name: " << setName << ". No entry in texCoords for that key.\n";

        return {-tileSize, -tileSize};
    }
}

std::string TileSet::getKeyFor(sf::Vector2f texCoord)
{
    for (auto entry : texCoords)
    {
        if (entry.second == texCoord)
        {
            return entry.first;
        }
    }

    std::cout << "ERROR in tile set, trying to find key for tex coords " << texCoord.x << ", " << texCoord.y << ". No entry in tex coord map.\n";
    assert(false);

    return "";
}

float TileSet::getTileSize() { return tileSize; }

std::string TileSet::getName() { return setName; }

bool TileSet::hasCenter()
{
    return texCoords.find("c") != texCoords.end();
}