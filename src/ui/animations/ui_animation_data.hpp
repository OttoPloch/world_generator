#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include "../ui_position.hpp"

struct UIAnimationData
{
    std::string name;
    
    float timeToComplete;
    bool relativePos;
    sf::Vector2f startPosition;
    sf::Vector2f endPosition;
    std::unique_ptr<UIOrigin> startOrigin;
    std::unique_ptr<UIOrigin> endOrigin;
    std::unique_ptr<UIAnchor> startAnchor;
    std::unique_ptr<UIAnchor> endAnchor;
};