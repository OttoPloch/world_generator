#pragma once

#include <memory>

#include "common.hpp"
#include "sprite.hpp"
#include "asset_manager.hpp"
#include "motion_attribute.hpp"
#include "game_position.hpp"

class Entity
{
public:
    Entity();

    void create(sf::Vector2f position);

    void giveSprite(sf::Texture* texture, sf::Vector2f size, bool centerOrigin = true);

    void giveMotion(bool controlling);

    void changeSpriteTexture(sf::Texture* texture);

    void tick();

    void update(float dt);

    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition();

    Sprite* getSprite();
private:
    GamePosition position;

    float rotation;

    std::unique_ptr<Sprite> sprite;

    std::unique_ptr<MotionAttribute> motion;
};