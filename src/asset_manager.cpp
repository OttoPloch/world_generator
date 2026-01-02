#include "asset_manager.hpp"
#include "states.hpp"

AssetManager::AssetManager() {}

sf::Texture* AssetManager::getTexture(std::string name, std::string pathFromAssets, bool fullPath)
{
    auto entry = textureMap.find(name);

    if (entry != textureMap.end())
    {
        return &entry->second;        
    }
    else
    {
        sf::Texture newTexture;

        if (fullPath)
        {
            if (!newTexture.loadFromFile("../../assets/" + pathFromAssets))
            {
                std::cout << "error loading " << name << " texture with FULL PATH of " << pathFromAssets << '\n';

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
                    if (!newTexture.loadFromFile("../../assets/" + pathFromAssets + name + ".png"))
                    {
                        std::cout << "error loading " << name << ".png with path from assets of " << pathFromAssets << '\n';
                        
                        return nullptr;
                    }
                }
            }
            else
            {
                if (!newTexture.loadFromFile("../../assets/" + pathFromAssets + name + ".jpg"))
                {
                    std::cout << "error loading " << name << ".jpg with path from assets of " << pathFromAssets << '\n';;
    
                    return nullptr;
                }
            }

        }

        textureMap[name] = newTexture;

        return &textureMap[name];
    }
}

Animation* AssetManager::getAnimation(std::string name)
{
    auto entry = animationMap.find(name);

    if (entry != animationMap.end())
    {
        return &entry->second;        
    }
    else
    {
        Animation newAnimation;

        if (!std::filesystem::exists("../../assets/animations/" + name + ".anim"))
        {
            std::cout << "error loading " << name << ".anim\n";

            return nullptr;
        }
        else
        {
            // load animation
            std::ifstream animFile("../../assets/animations/" + name + ".anim");

            std::string textureName;
            std::string texturePath;
            int texColumns;
            int texRows;
            float sizeFractionX;
            float sizeFractionY;
            int baseTicksPerFrame;
            sf::FloatRect collisionRect({0, 0}, {0, 0});

            std::string line;

            while (std::getline(animFile, line))
            {
                if (line.substr(0, 9) == "tex name ") textureName = line.substr(9);
                if (line.substr(0, 5) == "path ") texturePath = line.substr(5);
                if (line.substr(0, 4) == "col ") texColumns = toInt(std::stof(line.substr(4)));
                if (line.substr(0, 4) == "row ") texRows = toInt(std::stof(line.substr(4)));
                if (line.substr(0, 6) == "sizex ") sizeFractionX = std::stof(line.substr(6));
                if (line.substr(0, 6) == "sizey ") sizeFractionY = std::stof(line.substr(6));
                if (line.substr(0, 18) == "baseticksperframe ") baseTicksPerFrame = toInt(std::stof(line.substr(18)));
                if (line.substr(0, 11) == "colloffset ")
                {
                    std::string offset = line.substr(11);
                    int comma = offset.find(',');
                    float x = std::stof(offset.substr(0, comma));
                    float y = std::stof(offset.substr(comma + 1));

                    collisionRect.position = {x, y};
                }
                if (line.substr(0, 9) == "collsize ")
                {
                    std::string size = line.substr(9);
                    auto comma = size.find(',');
                    float x = std::stof(size.substr(0, comma));
                    float y = std::stof(size.substr(comma + 1));

                    collisionRect.size = {x, y};
                }
            }

            sf::Texture* animTexture = getTexture(textureName, texturePath, true);

            sf::Vector2u texSize = animTexture->getSize();

            int frameLength = texSize.x / texColumns;
            int frameHeight = texSize.y / texRows;

            int fittedFrameLength = toInt(frameLength * sizeFractionX);
            int fittedFrameHeight = toInt(frameHeight * sizeFractionY);

            std::vector<sf::Vector2i> animationFrames;

            for (int y = 0; y < texRows; y++)
            {
                for (int x = 0; x < texColumns; x++)
                {
                    float xOffset = std::ceil((frameLength * ((1.f - sizeFractionX) / 2.f)));
                    float yOffset = std::ceil((frameHeight * ((1.f - sizeFractionY) / 2.f)));

                    animationFrames.push_back({toInt(toFloat(frameLength * x) + xOffset), toInt(toFloat(frameHeight * y) + yOffset)});
                }
            }

            newAnimation.init(name, baseTicksPerFrame, animTexture, {fittedFrameLength, fittedFrameHeight}, animationFrames, collisionRect);
        }

        animationMap[name] = newAnimation;

        return &animationMap[name];
    }
}

AnimationSet* AssetManager::getAnimSet(std::string name)
{
    auto entry = animSetMap.find(name);

    if (entry != animSetMap.end())
    {
        return &entry->second;        
    }
    else
    {
        AnimationSet newSet;

        if (!std::filesystem::exists("../../assets/animations/sets/" + name + ".animset"))
        {
            std::cout << "error loading " << name << ".animset\n";

            return nullptr;
        }
        else
        {
            // load animation set
            std::ifstream setFile("../../assets/animations/sets/" + name + ".animset");

            std::unordered_map<int, Animation*> animations;

            std::vector<int> states;
            std::vector<std::string> animNames;

            std::string line;

            while (std::getline(setFile, line))
            {
                if (line.substr(0, 5) == "state") states.push_back(animationStringToState[line.substr(6)]);
                if (line.substr(0, 4) == "anim") animNames.push_back(line.substr(5));
            }

            for (int i = 0; i < states.size(); i++)
            {
                animations[states[i]] = getAnimation(animNames[i]);
            }

            newSet.init(name, animations);
        }

        animSetMap[name] = newSet;

        return &animSetMap[name];
    }
}

TileSet* AssetManager::getTileSet(std::string name)
{
    auto entry = tileSetMap.find(name);

    if (entry != tileSetMap.end())
    {
        return &entry->second;        
    }
    else
    {
        TileSet newSet;

        if (!std::filesystem::exists("../../assets/tilesets/" + name + ".tileset"))
        {
            std::cout << "error loading " << name << ".tileset\n";

            return nullptr;
        }
        else
        {
            // load tile set
            std::ifstream setFile("../../assets/tilesets/" + name + ".tileset");

            std::unordered_map<std::string, sf::Vector2f> texCoords;

            sf::Texture* texture;
            
            std::string texName;
            std::string texPath;
            float tileSize;

            std::vector<std::string> locations;
            std::vector<float> xCoords;
            std::vector<float> yCoords;

            std::string line;

            while (std::getline(setFile, line))
            {
                if (line.substr(0, 7) == "texture") texName = line.substr(8);
                if (line.substr(0, 4) == "path") texPath = line.substr(5);
                if (line.substr(0, 8) == "tilesize") tileSize = std::stof(line.substr(9));
                if (line.substr(0, 8) == "location") locations.push_back(line.substr(9));
                if (line.substr(0, 6) == "xCoord") xCoords.push_back(std::stof(line.substr(7)));
                if (line.substr(0, 6) == "yCoord") yCoords.push_back(std::stof(line.substr(7)));
            }

            texture = getTexture(texName, texPath, true);

            for (int i = 0; i < locations.size(); i++)
            {
                texCoords[locations[i]] = {xCoords[i], yCoords[i]};
            }

            newSet.init(name, texture, texCoords, tileSize);
        }

        tileSetMap[name] = newSet;

        return &tileSetMap[name];
    }
}