#pragma once

#include "common.hpp"
#include "world_position.hpp"
#include "animation.hpp"
#include "animation_set.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

class Sprite
{
public:
    Sprite();

    Sprite(WorldPosition position, sf::Texture* texture, sf::Vector2f size = {1.f, 1.f}, bool sizeIsScale = true, bool usingTexCoords = false, sf::IntRect texCoords = sf::IntRect({0, 0}, {0, 0}));

    sf::Vector2f getPosition();
    sf::Vector2f getSize();

    float left();
    float right();
    float top();
    float bottom();

    void resize(sf::Vector2f newSize, bool sizeIsScale = true);
    void setTextureRect(sf::IntRect newTexRect);

    void syncPos(sf::Vector2f interpolatedPos = {0, 0}, bool useInterpolated = false);

    void update(float dt);

    void draw(sf::RenderWindow& window);

    std::unique_ptr<sf::Sprite> sprite;

    std::unique_ptr<Animation> animation;
    std::unique_ptr<AnimationSet> animSet;
private:
    WorldPosition position;

    sf::Vector2f size;

    sf::Texture* texture;
};
