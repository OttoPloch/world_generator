#pragma once

#include <memory>

#include "common.hpp"
#include "sprite.hpp"
#include "asset_manager.hpp"
#include "motion_attribute.hpp"

class Entity
{
public:
    Entity();

    void create(sf::Vector2f position);

    void giveSprite(sf::Texture* texture, sf::Vector2f size, bool centerOrigin = true);

    void giveMotion();

    void changeSpriteTexture(sf::Texture* texture);

    void tick();

    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition();

    const Sprite& getSprite();
private:
    sf::Vector2f position;

    float rotation;

    std::unique_ptr<Sprite> sprite;

    std::unique_ptr<MotionAttribute> motion;
};