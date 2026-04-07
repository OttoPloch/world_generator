#pragma once

#include "../core/common.hpp"
#include <SFML/Graphics/Rect.hpp>

struct Animation
{
    Animation();

    Animation(std::string name, sf::Texture* texture, std::vector<sf::IntRect> frames, float secondsPerFrame = 0.5f);

    void adjustSpeed(float newSecondsPerFrame);

    std::string name;
    sf::Texture* texture;
    std::vector<sf::IntRect> frames;

    int index;
    float secondsPerFrame;
    float secondsTillNextFrame;

    bool reversed;
};