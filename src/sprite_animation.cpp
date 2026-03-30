#include "sprite_animation.hpp"

SpriteAnimation::SpriteAnimation() {}

void SpriteAnimation::init(std::string name, int baseTicksPerFrame, sf::Texture* texture, sf::Vector2i frameSize, std::vector<sf::Vector2i> frames, sf::FloatRect collisionRect)
{
    this->name = name;
    this->baseTicksPerFrame = baseTicksPerFrame;
    this->texture = texture;
    this->frameSize = frameSize;
    this->frames = frames;
    this->collisionRect = collisionRect;
}

sf::Texture* SpriteAnimation::getTexture() { return texture; }

sf::Vector2i SpriteAnimation::getFrameSize() { return frameSize; }

sf::Vector2i SpriteAnimation::getFrameCoords(int index) { return frames[index]; }

int SpriteAnimation::getFrameCount() { return frames.size(); }

std::string SpriteAnimation::getName() { return name; }

int SpriteAnimation::getBaseTicksPerFrame() { return baseTicksPerFrame; }

bool SpriteAnimation::hasCollisionRect() { return (collisionRect != sf::FloatRect({0, 0}, {0, 0})); }

sf::FloatRect SpriteAnimation::getCollisionRect() { return collisionRect; }