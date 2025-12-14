#include "asset_manager.hpp"

AssetManager::AssetManager() {}

sf::Texture* AssetManager::getTexture(std::string name)
{
    auto entry = textureMap.find(name);

    if (entry != textureMap.end())
    {
        return &entry->second;        
    }
    else
    {
        sf::Texture newTexture;

        if (!newTexture.loadFromFile("../../assets/images/" + name + ".jpg"))
        {
            if (!newTexture.loadFromFile("../../assets/images/" + name + ".png"))
            {
                std::cout << "Error loading " << name << " image, not a jpg or png.\n";   

                return nullptr;
            }
        }

        textureMap[name] = newTexture;

        return &textureMap[name];
    }
}