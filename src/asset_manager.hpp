#pragma once

#include <map>
#include <vector>

#include "common.hpp"

class AssetManager
{
public:
    AssetManager();

    sf::Texture* getTexture(std::string name);
private:
    std::map<std::string, sf::Texture> textureMap;
};