#pragma once

#include "common.hpp"
#include "sprite.hpp"

#include <vector>

class Animation
{
public:
    Animation();

    void init(std::string name, int baseTicksPerFrame, sf::Texture* texture, sf::Vector2i frameSize, std::vector<sf::Vector2i> frames);

    sf::Texture* getTexture();

    sf::Vector2i getFrameSize();

    sf::Vector2i getFrameCoords(int index);

    int getFrameCount();

    std::string getName();

    int getBaseTicksPerFrame();
private:
    std::string name;

    int baseTicksPerFrame;

    sf::Texture* texture;

    sf::Vector2i frameSize;

    std::vector<sf::Vector2i> frames;
};