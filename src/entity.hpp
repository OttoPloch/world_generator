#pragma once

#include "common.hpp"
#include "sprite.hpp"
#include "asset_manager.hpp"

class Entity
{
public:
    Entity();

    void create(sf::Vector2f position);

    void giveSprite(sf::Texture* texture, sf::Vector2f size, bool centerOrigin = true);

    void changeSpriteTexture(sf::Texture* texture);

    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition();

    const Sprite& getSprite();
private:
    sf::Vector2f position;

    float rotation;

    Sprite sprite;
};