#pragma once

#include "common.hpp"
#include "game_position.hpp"
#include "animation_set.hpp"
#include "entity_states.hpp"
#include "gamerules.hpp"

#include <memory>

class Animation;
class Entity;
class Game;

class Sprite
{
public:
    Sprite();

    // sprite will always be centered if an animation is given, so centerOrigin only truly affects entities with no animations.
    void create(Game* game, Entity* myEntity, sf::Texture* texture, GamePosition position, sf::Vector2f size, int z = 0, bool centerOrigin = true);

    void centerSprite();

    void setSize(sf::Vector2f newSize);

    void setRotation(float newRotation);

    void setTexture(sf::Texture* newTexture);

    void giveAnimationSet(AnimationSet* animationSet, bool resetSizeX = true);

    void giveAnimation(Animation* animation, bool resetSizeX = true, bool reverse = false, bool start = true);

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

    int getZ();
    
    void jumpToTarget();
private:
    void changeAnimation(Animation* newAnimation);

    Gamerules* gamerules;

    Entity* myEntity;

    EntityStates* states;

    GamePosition position;

    sf::Vector2f spritePosition;

    sf::Vector2f size;

    float rotation;

    sf::Texture* texture;

    // needs to be a ptr bc of no default constructor for sf::Sprite
    std::unique_ptr<sf::Sprite> sprite;

    int z;

    AnimationSet* animationSet;

    Animation* animation;
    int animTicksPerFrame;
    bool animReverse;
    unsigned int animTicksToNextFrame;
    unsigned int animFrameIndex;
    bool animPlaying;
};