#include "ui_animation_data.hpp"

UIAnimationData::UIAnimationData() :
name(""),
timeToComplete(0),
startPosition(0, 0),
endPosition(0, 0)
{}

UIAnimationData::UIAnimationData(std::string name, float timeToComplete, sf::Vector2f startPosition, sf::Vector2f endPosition) :
name(name),
timeToComplete(timeToComplete),
startPosition(startPosition),
endPosition(endPosition)
{}