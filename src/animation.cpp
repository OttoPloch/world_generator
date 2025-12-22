#include "animation.hpp"

Animation::Animation() {}

void Animation::init(std::string name, int baseTicksPerFrame, sf::Texture* texture, sf::Vector2i frameSize, std::vector<sf::Vector2i> frames)
{
    this->name = name;

    this->baseTicksPerFrame = baseTicksPerFrame;

    this->texture = texture;

    this->frameSize = frameSize;

    this->frames = frames;
}

sf::Texture* Animation::getTexture() { return texture; }

sf::Vector2i Animation::getFrameSize() { return frameSize; }

sf::Vector2i Animation::getFrameCoords(int index) { return frames[index]; }

int Animation::getFrameCount() { return frames.size(); }

std::string Animation::getName() { return name; }

int Animation::getBaseTicksPerFrame() { return baseTicksPerFrame; }