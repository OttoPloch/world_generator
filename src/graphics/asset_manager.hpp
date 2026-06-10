#pragma once

#include "../core/common.hpp"
#include "animation.hpp"
#include "global_animation.hpp"
#include "animation_set.hpp"
#include "texture_atlas.hpp"
#include "../ui/animations/ui_animation_data.hpp"

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

class AssetManager
{
public:
    AssetManager();

    sf::Texture* getTexture(std::string name, std::string pathFromAssets = "images/", bool pathIncludesTheFile = false);

    Animation* getAnimation(std::string name, std::string pathFromAssets = "animations/");

    GlobalAnimation* getGlobalAnimation(std::string name);

    AnimationSet* getAnimSet(std::string name);

    sf::Font* getFont(std::string name);

    TextureAtlas* getTextureAtlas(std::string name, std::string pathFromTextureAtlases = "");

    UIAnimationData* getUIAnimationData(std::string name);

    void updateGlobalAnimations(float dt);
private:
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textureMap;

    std::unordered_map<std::string, std::unique_ptr<Animation>> animationMap;
    
    std::unordered_map<std::string, std::unique_ptr<GlobalAnimation>> globalAnimationMap;

    std::unordered_map<std::string, std::unique_ptr<AnimationSet>> animSetMap;

    std::unordered_map<std::string, sf::Font> fontMap;

    std::unordered_map<std::string, TextureAtlas> atlasMap;

    std::unordered_map<std::string, UIAnimationData> UIAnimationDataMap;
};