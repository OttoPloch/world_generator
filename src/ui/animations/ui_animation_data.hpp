#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>

struct UIAnimationData
{
    UIAnimationData();

    UIAnimationData(std::string name, float timeToComplete, sf::Vector2f startPosition, sf::Vector2f endPosition);

    std::string name;
    
    float timeToComplete;
    sf::Vector2f startPosition;
    sf::Vector2f endPosition;
};