#include "asset_manager.hpp"

AssetManager::AssetManager()
{
    if (!pixelTexture.loadFromFile("../../assets/images/image.png")) std::cout << "Error loading image.png\n";
    if (!shaqTexture.loadFromFile("../../assets/images/shaq_time_out.png")) std::cout << "Error loading shaq_time_out.png\n";

    textures = {
        {"pixel", &pixelTexture},
        {"shaq", &shaqTexture}
    };
}

sf::Texture* AssetManager::getTexture(std::string path)
{
    if (textures[path])
    {
        return textures[path];        
    }
    
    return nullptr;
}