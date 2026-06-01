#include "animation.hpp"
#include <SFML/Graphics/Rect.hpp>

Animation::Animation() : name("") {}

Animation::Animation(std::string name, sf::Texture* texture, std::vector<sf::FloatRect> frames, float secondsPerFrame) : name(name), texture(texture), frames(frames)
{
    index = 0;
    this->secondsPerFrame = secondsPerFrame;
    secondsTillNextFrame = secondsPerFrame;

    reversed = false;
}

void Animation::adjustSpeed(float newSecondsPerFrame)
{
    secondsPerFrame = newSecondsPerFrame;
    secondsTillNextFrame = std::min(newSecondsPerFrame, secondsTillNextFrame);
}