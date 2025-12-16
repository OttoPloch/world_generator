#include "asset_manager.hpp"

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
        }
        else
        {
            // load animation
            std::ifstream animFile("../../assets/animations/" + name + ".anim");

            std::string textureName;
            std::string texturePath;
            int texColumns;
            int texRows;

            std::string line;


            while (std::getline(animFile, line))
            {
                if (line.substr(0, 8) == "tex name") textureName = line.substr(9);
                if (line.substr(0, 4) == "path") texturePath = line.substr(5);
                if (line.substr(0, 3) == "col") texColumns = toInt(std::stof(line.substr(4)));
                if (line.substr(0, 3) == "row") texRows = toInt(std::stof(line.substr(4)));
            }

            sf::Texture* animTexture = getTexture(textureName, texturePath, true);

            sf::Vector2u texSize = animTexture->getSize();

            int frameLength = texSize.x / texColumns;
            int frameHeight = texSize.y / texRows;

            std::vector<sf::Vector2i> animationFrames;

            for (int y = 0; y < texRows; y++)
            {
                for (int x = 0; x < texColumns; x++)
                {
                    animationFrames.push_back({frameLength * x, frameHeight * y});
                }
            }

            newAnimation.init(name, animTexture, {frameLength, frameHeight}, animationFrames);
        }

        animationMap[name] = newAnimation;

        return &animationMap[name];
    }
}