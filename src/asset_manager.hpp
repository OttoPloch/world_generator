#pragma once

#include "common.hpp"
#include "animation.hpp"
#include "global_animation.hpp"
#include "tile_set.hpp"
#include "texture_atlas.hpp"

#include <map>
#include <vector>
#include <filesystem>
#include <fstream>

class AssetManager
{
public:
    AssetManager();

    sf::Texture* getTexture(std::string name, std::string pathFromAssets = "images/", bool pathIncludesTheFile = false);

    Animation* getAnimation(std::string name);

    GlobalAnimation* getGlobalAnimation(std::string name);

    // AnimationSet* getAnimSet(std::string name);

    TileSet* getTileSet(std::string name);

    sf::Font* getFont(std::string name);

    TextureAtlas* getTextureAtlas(std::string name);

    void updateGlobalAnimations(float dt);
private:
    std::unordered_map<std::string, sf::Texture> textureMap;

    std::unordered_map<std::string, std::unique_ptr<Animation>> animationMap;
    
    std::unordered_map<std::string, std::unique_ptr<GlobalAnimation>> globalAnimationMap;

    // std::unordered_map<std::string, AnimationSet> animSetMap;

    std::unordered_map<std::string, TileSet> tileSetMap;

    std::unordered_map<std::string, sf::Font> fontMap;

    std::unordered_map<std::string, TextureAtlas> atlasMap;
};