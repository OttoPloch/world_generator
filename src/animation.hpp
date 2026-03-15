#pragma once

#include "common.hpp"
#include <SFML/Graphics/Rect.hpp>

struct Animation
{
    Animation();

    Animation(std::string name, sf::Texture* texture, std::vector<sf::IntRect> frames);

    void adjustSpeed(float newSecondsPerFrame);

    std::string name;
    sf::Texture* texture;
    std::vector<sf::IntRect> frames;

    int index;
    float secondsPerFrame;
    float secondsTillNextFrame;

    bool reversed;
};