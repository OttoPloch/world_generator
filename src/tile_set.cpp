#include "tile_set.hpp"

TileSet::TileSet() {}

TileSet::TileSet(std::string setName, sf::Texture* texture, std::unordered_map<std::string, sf::Vector2f> texCoords)
{
    init(setName, texture, texCoords);
}

void TileSet::init(std::string setName, sf::Texture* texture, std::unordered_map<std::string, sf::Vector2f> texCoords)
{
    this->setName = setName;

    this->texture = texture;

    this->texCoords = texCoords;
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

        return texCoords["missing"];
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

sf::Texture* TileSet::getTexture() { return texture; }