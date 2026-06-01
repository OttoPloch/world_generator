#include "texture_atlas.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <unordered_map>

TextureAtlas::TextureAtlas() {}

TextureAtlas::TextureAtlas(std::string atlasName, std::unordered_map<std::string, sf::FloatRect> itemTexCoords) : name(atlasName), itemTexCoords(itemTexCoords) {}

sf::FloatRect TextureAtlas::getItemTexCoords(std::string item)
{
    auto entry = itemTexCoords.find(item);

    if (entry != itemTexCoords.end())
    {
        return entry->second;
    }

    std::cout << "ERROR getting tex coords for atlas " << name << " item " << item << ". Could not find that item in that texture atlas.\n";

    return sf::FloatRect({0, 0}, {0, 0});
}