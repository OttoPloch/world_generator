#pragma once

#include "../core/common.hpp"
#include "../utils/game_position.hpp"
#include "animation.hpp"
#include "animation_set.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

class Sprite
{
public:
    Sprite();

    Sprite(GamePosition position, sf::Texture* texture, sf::Vector2f size = {1.f, 1.f}, bool sizeIsScale = true, bool usingTexCoords = false, sf::IntRect texCoords = sf::IntRect({0, 0}, {0, 0}), float animSpeedMult = 1.f, bool repeatAnimation = true);

    sf::Vector2f getPosition();
    sf::Vector2f getSize();

    GamePosition getPositionVar();

    float left();
    float right();
    float top();
    float bottom();

    void setTexture(sf::Texture* newTexture);

    void resize(sf::Vector2f newSize, bool sizeIsScale = true);
    void setTextureRect(sf::IntRect newTexRect);

    void syncPos();

    void update(float dt);

    void draw(sf::RenderWindow& window);

    std::unique_ptr<sf::Sprite> sprite;

    std::unique_ptr<Animation> animation;
    std::unique_ptr<AnimationSet> animSet;
    float animSpeedMult;
    bool repeatAnimation;
private:
    GamePosition position;

    sf::Vector2f size;
    bool sizeIsScale;

    sf::Texture* texture;
};
