#pragma once

#include <map>

#include "common.hpp"

class AssetManager
{
public:
    AssetManager();

    sf::Texture* getTexture(std::string name);
private:
    std::map<std::string, sf::Texture*> textures;

    sf::Texture pixelTexture;

    sf::Texture shaqTexture;
};