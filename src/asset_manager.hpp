#pragma once

#include "common.hpp"
#include "sprite_animation.hpp"
#include "animation_set.hpp"
#include "tile_set.hpp"

#include <map>
#include <vector>
#include <filesystem>
#include <fstream>

class AssetManager
{
public:
    AssetManager();

    sf::Texture* getTexture(std::string name, std::string pathFromAssets = "images/", bool fullPath = false);

    SpriteAnimation* getAnimation(std::string name);

    AnimationSet* getAnimSet(std::string name);

    TileSet* getTileSet(std::string name);

    sf::Font* getFont(std::string name);
private:
    std::unordered_map<std::string, sf::Texture> textureMap;

    std::unordered_map<std::string, SpriteAnimation> animationMap;

    std::unordered_map<std::string, AnimationSet> animSetMap;

    std::unordered_map<std::string, TileSet> tileSetMap;

    std::unordered_map<std::string, sf::Font> fontMap;
};