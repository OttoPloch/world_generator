#pragma once

#include <memory>

#include "common.hpp"
#include "game_position.hpp"

//temp
#include "motion_attribute.hpp"

class Sprite
{
public:
    Sprite();

    ~Sprite();

    void create(sf::Texture* texture, GamePosition position, sf::Vector2f size, bool centerOrigin = true);

    void setSize(sf::Vector2f newSize);

    void setRotation(float newRotation);

    void setTexture(sf::Texture* newTexture);

    void tick();

    void update(float dt, MotionAttribute* a);

    void draw(sf::RenderWindow& window);
    
    sf::Sprite getSprite();

    sf::Vector2f getSpritePosition();
private:
    GamePosition position;

    sf::Vector2f spritePosition;

    sf::Vector2f size;

    float rotation;

    sf::Texture* texture;
    
    // needs to be a ptr bc of no default constructor for sf::Sprite
    std::unique_ptr<sf::Sprite> sprite;
};