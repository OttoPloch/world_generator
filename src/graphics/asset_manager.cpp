#include "asset_manager.hpp"
#include "texture_atlas.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <fstream>

AssetManager::AssetManager() {}

sf::Texture* AssetManager::getTexture(std::string name, std::string pathFromAssets, bool pathIncludesTheFile)
{
    auto entry = textureMap.find(name);

    if (entry != textureMap.end())
    {
        return entry->second.get();
    }
    else
    {
        std::unique_ptr<sf::Texture> newTexture = std::make_unique<sf::Texture>();

        if (pathIncludesTheFile)
        {
            if (!newTexture->loadFromFile("../../assets/" + pathFromAssets))
            {
                std::cout << "error loading texture with FULL PATH of " << pathFromAssets << '\n';

                return nullptr;
            }
        }
        else
        {
            if (!std::filesystem::exists("../../assets/" + pathFromAssets + name + ".jpg"))
            {
                if (!std::filesystem::exists("../../assets/" + pathFromAssets + name + ".png"))
                {
                    std::cout << "Error loading " << name << " image with path from assets of " << pathFromAssets << ", not a jpg or png.\n";   
    
                    return nullptr;
                }
                else
                {
                    if (!newTexture->loadFromFile("../../assets/" + pathFromAssets + name + ".png"))
                    {
                        std::cout << "error loading " << name << ".png with path from assets of " << pathFromAssets << '\n';
                        
                        return nullptr;
                    }
                }
            }
            else
            {
                if (!newTexture->loadFromFile("../../assets/" + pathFromAssets + name + ".jpg"))
                {
                    std::cout << "error loading " << name << ".jpg with path from assets of " << pathFromAssets << '\n';;
    
                    return nullptr;
                }
            }
        }

        newTexture->setSmooth(false);

        textureMap[name] = std::move(newTexture);

        return textureMap[name].get();
    }
}

Animation* AssetManager::getAnimation(std::string name, std::string pathFromAssets)
{
    auto entry = animationMap.find(name);

    if (entry != animationMap.end())
    {
        return entry->second.get();
    }
    else
    {
        std::unique_ptr<Animation> newAnimation;

        if (!std::filesystem::exists("../../assets/" + pathFromAssets + name + ".anim"))
        {
            std::cout << "error loading " << pathFromAssets << name << ".anim, doesn't exist.\n";
        }
        else
        {
            std::ifstream animFile("../../assets/" + pathFromAssets + name + ".anim");

            std::string textureName = "missing";
            std::string texturePath = "images/";
            float defaultSPF = 0.5f; // SPF = seconds per frame
            std::vector<sf::Vector2f> coords;
            std::vector<sf::Vector2f> sizes;

            std::string line;

            while (std::getline(animFile, line))
            {
                if (line.substr(0, 12) == "texture name") textureName = line.substr(13);
                if (line.substr(0, 4) == "path") texturePath = line.substr(5);
                if (line.substr(0, 11) == "default spf") defaultSPF = std::stof(line.substr(12));
                if (line.substr(0, 5) == "coord")
                {
                    std::string substr = line.substr(6);
                    int commaIndex = substr.find(',');
                    if (commaIndex != std::string::npos)
                    {
                        float coordX = std::floor(std::stof(substr.substr(0, commaIndex)));
                        float coordY = std::floor(std::stof(substr.substr(commaIndex + 1)));

                        coords.push_back({coordX, coordY});
                    }
                    else
                    {
                        coords.push_back({0, 0});

                        std::cout << "error getting tex coords of a frame in animation called " << name << ". Make sure the animation file has coords typed properly (coord x,y)";
                    }
                }
                if (line.substr(0, 4) == "size")
                {
                    std::string substr = line.substr(5);
                    int commaIndex = substr.find(',');
                    if (commaIndex != std::string::npos)
                    {
                        float sizeX = std::floor(std::stof(substr.substr(0, commaIndex)));
                        float sizeY = std::floor(std::stof(substr.substr(commaIndex + 1)));

                        sizes.push_back({sizeX, sizeY});
                    }
                    else
                    {
                        sizes.push_back({0, 0});

                        std::cout << "error getting size of a frame in animation called " << name << ". Make sure the animation file has sizes typed properly (size x,y).\n";
                    }
                }
            }

            animFile.close();
            
            sf::Texture* animTexture = getTexture(textureName, texturePath);

            std::vector<sf::FloatRect> frames;

            for(int i = 0; i < coords.size(); i++) frames.emplace_back(coords[i], sizes[i]);

            newAnimation = std::make_unique<Animation>(name, animTexture, frames, defaultSPF);
        }

        animationMap[name] = std::move(newAnimation);
        
        return animationMap[name].get();
    }

    return nullptr;
}

GlobalAnimation* AssetManager::getGlobalAnimation(std::string name)
{
    auto entry = globalAnimationMap.find(name);

    if (entry != globalAnimationMap.end())
    {
        return entry->second.get();
    }
    else
    {
        Animation* animation = getAnimation(name);

        if (animation)
        {
            globalAnimationMap[name] = std::make_unique<GlobalAnimation>(*animation);

            return globalAnimationMap[name].get();
        }
        else
        {
            std::cout << "could not create global animation of animation with name " << name << ". That animation could not be retrieved.\n";
        }
    }
    
    return nullptr;
}

AnimationSet* AssetManager::getAnimSet(std::string name)
{
    auto entry = animSetMap.find(name);

    if (entry != animSetMap.end())
    {
        return entry->second.get();
    }
    else
    {
        std::unique_ptr<AnimationSet> newSet;

        if (!std::filesystem::exists("../../assets/animations/sets/" + name + ".animset"))
        {
            std::cout << "error loading " << name << ".animset\n";

            return nullptr;
        }
        else
        {
            // load animation set
            std::ifstream setFile("../../assets/animations/sets/" + name + ".animset");

            std::unordered_map<AnimState, Animation> animations;

            std::string animPath = "";
            std::vector<AnimState> keys;
            std::vector<std::string> animNames;

            std::string line;

            while (std::getline(setFile, line))
            {
                if (line.substr(0, 3) == "dir") animPath = line.substr(4);
                if (line.substr(0, 3) == "key")
                {
                    if (toInt(std::stof(line.substr(4))) < enumSize<AnimState>())
                    {
                        keys.push_back(static_cast<AnimState>(toInt(std::stof(line.substr(4)))));
                    }
                }
                if (line.substr(0, 4) == "anim") animNames.push_back(line.substr(5));
            }
            
            setFile.close();
            
            for (int i = 0; i < keys.size(); i++)
            {
            animations[keys[i]] = *getAnimation(animNames[i], animPath);
            }

            newSet = std::make_unique<AnimationSet>(name, animations);
        }

        animSetMap[name] = std::move(newSet);

        return animSetMap[name].get();
    }
}

sf::Font* AssetManager::getFont(std::string name)
{
    auto entry = fontMap.find(name);

    if (entry != fontMap.end())
    {
        return &entry->second;
    }
    else
    {
        sf::Font newFont;

        if (!std::filesystem::exists("../../assets/text/" + name + ".otf"))
        {
            if (!std::filesystem::exists("../../assets/text/" + name + ".ttf"))
            {
                std::cout << "Error loading " << name << " font. No ttf or otf file in assets/text/ with that name.\n";   
    
                return nullptr;
            }
            else
            {
                if (!newFont.openFromFile("../../assets/text/" + name + ".ttf"))
                {
                    std::cout << "error loading " << name << ".ttf\n";
                    
                    return nullptr;
                }
            }
        }
        else
        {
            if (!newFont.openFromFile("../../assets/text/" + name + ".otf"))
            {
                std::cout << "error loading " << name << ".otf\n";
                
                return nullptr;
            }
        }

        fontMap[name] = newFont;

        return &fontMap[name];
    }
}

TextureAtlas* AssetManager::getTextureAtlas(std::string name, std::string pathFromTextureAtlases)
{
    auto entry = atlasMap.find(name);

    if (entry != atlasMap.end())
    {
        return &entry->second;
    }
    else
    {
        TextureAtlas newAtlas;

        if (!std::filesystem::exists("../../assets/texture_atlases/" + pathFromTextureAtlases + name + ".atlas"))
        {
            std::cout << "error loading " << name << ".atlas with pathFromTextureAtlases of '" << pathFromTextureAtlases << "'.\n";

            return nullptr;
        }
        else
        {
            // load atlas
            std::ifstream atlasFile("../../assets/texture_atlases/" + pathFromTextureAtlases + name + ".atlas");

            std::unordered_map<std::string, sf::FloatRect> texCoords;

            // makes it simpler to type in the positions of
            // atlases when making them, reduces to multiples.
            unsigned int tileSize;

            std::vector<std::string> items;
            std::vector<sf::Vector2f> coords;
            std::vector<sf::Vector2f> sizes;

            std::string line;

            while (std::getline(atlasFile, line))
            {
                if (line.substr(0, 8) == "tilesize") tileSize = toUnsignedInt(std::stof(line.substr(9)));
                if (line.substr(0, 4) == "item") items.push_back(line.substr(5));
                if (line.substr(0, 5) == "coord")
                {
                    std::string substr = line.substr(6);
                    int commaIndex = substr.find(',');
                    if (commaIndex != std::string::npos)
                    {
                        float coordX = std::floor(std::stof(substr.substr(0, commaIndex)));
                        float coordY = std::floor(std::stof(substr.substr(commaIndex + 1)));

                        coords.push_back({coordX, coordY});
                    }
                    else
                    {
                        coords.push_back({0, 0});

                        std::cout << "error getting tex coords of an item in texture atlas called " << name << ". Make sure the .atlas file has coords typed properly ('coord x,y')";
                    }
                }
                if (line.substr(0, 4) == "size")
                {
                    std::string substr = line.substr(5);
                    int commaIndex = substr.find(',');
                    if (commaIndex != std::string::npos)
                    {
                        float sizeX = std::floor(std::stof(substr.substr(0, commaIndex)));
                        float sizeY = std::floor(std::stof(substr.substr(commaIndex + 1)));

                        sizes.push_back({sizeX, sizeY});
                    }
                    else
                    {
                        sizes.push_back({0, 0});

                        std::cout << "error getting size of an item in texture atlas called " << name << ". Make sure the atlas file has sizes typed properly ('size x,y').\n";
                    }
                }
            }

            atlasFile.close();

            for (int i = 0; i < items.size(); i++)
            {
                if (coords[i].x < 0) coords[i].x = 0;
                if (coords[i].y < 0) coords[i].y = 0;

                coords[i] = {coords[i].x * tileSize, coords[i].y * tileSize};
                sizes[i] = {sizes[i].x * tileSize, sizes[i].y * tileSize};

                texCoords[items[i]] = sf::FloatRect(coords[i], sizes[i]);
            }

            newAtlas = TextureAtlas(name, texCoords, tileSize);
        }

        atlasMap[name] = newAtlas;

        return &atlasMap[name];
    }
}

UIAnimationData* AssetManager::getUIAnimationData(std::string name)
{
    auto entry = UIAnimationDataMap.find(name);

    if (entry != UIAnimationDataMap.end())
    {
        return &entry->second;
    }
    else
    {
        UIAnimationData newData = {};

        if (!std::filesystem::exists("../../assets/ui_animations/" + name + ".uianim"))
        {
            std::cout << "ERROR loading UI Animation with name '" << name << "'. No .uianim file in assets/ui_animations/ with that name.\n";
            
            return nullptr;
        }
        else
        {
            // load animation
            std::ifstream dataFile("../../assets/ui_animations/" + name + ".uianim");

            float timeToComplete;
            sf::Vector2f startPosition;
            sf::Vector2f endPosition;

            std::string line;

            while (std::getline(dataFile, line))
            {
                if (line.substr(0, 4) == "time") timeToComplete = std::stof(line.substr(5));
                if (line.substr(0, 8) == "startpos")
                {
                    std::string substr = line.substr(9);
                    int commaIndex = substr.find(',');
                    if (commaIndex != std::string::npos)
                    {
                        float posX = std::floor(std::stof(substr.substr(0, commaIndex)));
                        float posY = std::floor(std::stof(substr.substr(commaIndex + 1)));

                        startPosition = {posX, posY};
                    }
                    else
                    {
                        startPosition = {0, 0};

                        std::cout << "error getting start position of ui animation data with name '" << name << "'. Make sure the .uianim file has its start position typed properly ('startpos x,y')";
                    }
                }
                if (line.substr(0, 6) == "endpos")
                {
                    std::string substr = line.substr(7);
                    int commaIndex = substr.find(',');
                    if (commaIndex != std::string::npos)
                    {
                        float posX = std::floor(std::stof(substr.substr(0, commaIndex)));
                        float posY = std::floor(std::stof(substr.substr(commaIndex + 1)));

                        endPosition = {posX, posY};
                    }
                    else
                    {
                        endPosition = {0, 0};

                        std::cout << "error getting end position of ui animation data with name '" << name << "'. Make sure the .uianim file has its end position typed properly ('endpos x,y')";
                    }
                }
            }

            dataFile.close();

            newData = UIAnimationData(name, timeToComplete, startPosition, endPosition);
        }

        UIAnimationDataMap[name] = newData;

        return &UIAnimationDataMap[name];
    }
}

void AssetManager::updateGlobalAnimations(float dt)
{
    for (auto& i : globalAnimationMap)
    {
        i.second->update(dt);
    }
}