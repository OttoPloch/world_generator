#include "asset_manager.hpp"

AssetManager::AssetManager()
{
    if (!pixelTexture.loadFromFile("../../assets/images/image.png")) std::cout << "Error loading image.png\n";
    if (!shaqTexture.loadFromFile("../../assets/images/shaq_time_out.png")) std::cout << "Error loading shaq_time_out.png\n";
    if (!drBeeTexture.loadFromFile("../../assets/images/dr bee.jpg")) std::cout << "Error loading dr bee.jpg\n";

    textures = {
        {"pixel", &pixelTexture},
        {"shaq", &shaqTexture},
        {"dr bee", &drBeeTexture}
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