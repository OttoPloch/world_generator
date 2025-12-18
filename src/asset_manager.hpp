#pragma once

#include "common.hpp"
#include "animation.hpp"
#include "animation_set.hpp"

#include <map>
#include <vector>
#include <filesystem>
#include <fstream>

class AssetManager
{
public:
    AssetManager();

    sf::Texture* getTexture(std::string name, std::string pathFromAssets = "images/", bool fullPath = false);

    Animation* getAnimation(std::string name);

    AnimationSet* getAnimSet(std::string name);
private:
    std::map<std::string, sf::Texture> textureMap;

    std::map<std::string, Animation> animationMap;

    std::map<std::string, AnimationSet> animSetMap;
};