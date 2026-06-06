#include "texture_atlas.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <unordered_map>

TextureAtlas::TextureAtlas() {}

TextureAtlas::TextureAtlas(std::string atlasName, std::unordered_map<std::string, sf::FloatRect> itemTexCoords, unsigned int tileSize) : name(atlasName), itemTexCoords(itemTexCoords), tileSize(tileSize) {}