#pragma once

#include "common.hpp"
#include "game_position.hpp"

#include <memory>

class Animation;

class Sprite
{
public:
    Sprite();

    void create(sf::Texture* texture, GamePosition position, sf::Vector2f size, bool centerOrigin = true);

    void centerSprite();

    void setSize(sf::Vector2f newSize);

    void setRotation(float newRotation);

    void setTexture(sf::Texture* newTexture);

    void giveAnimation(Animation* animation, unsigned int ticksPerFrame, bool reverse = false);

    void animPlay();

    void animStop();

    void animReset();

    void tick();

    void update(float dt);

    void draw(sf::RenderWindow& window);
    
    sf::Sprite getSprite();

    sf::Vector2f getSpritePosition();

    sf::Vector2f getSize();

    float getBottom();

    void jumpToTarget();
private:
    GamePosition position;

    sf::Vector2f spritePosition;

    sf::Vector2f size;

    float rotation;

    sf::Texture* texture;

    // needs to be a ptr bc of no default constructor for sf::Sprite
    std::unique_ptr<sf::Sprite> sprite;

    Animation* animation;
    float animTicksPerFrame;
    bool animReverse;
    unsigned int animTicksToNextFrame;
    unsigned int animFrameIndex;
    bool animPlaying;
};