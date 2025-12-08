#pragma once

#include <memory>
#include <vector>

#include "common.hpp"
#include "sprite.hpp"
#include "asset_manager.hpp"
#include "motion_attribute.hpp"
#include "game_position.hpp"

class CollisionAttribute;

class Entity
{
public:
    Entity();

    void create(int ID, sf::Vector2f position);

    int getID();

    void giveSprite(sf::Texture* texture, sf::Vector2f size, bool centerOrigin = true);

    void giveMotion(bool controlling);

    void giveCollision(std::vector<Entity>* entities, bool active);

    void changeSpriteTexture(sf::Texture* texture);

    void tick();

    void update(float dt);

    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition();

    Sprite* getSprite();

    MotionAttribute* getMotion();

    CollisionAttribute* getCollision();
private:
    int ID;

    GamePosition position;

    float rotation;

    std::unique_ptr<Sprite> sprite;

    std::unique_ptr<MotionAttribute> motion;

    std::unique_ptr<CollisionAttribute> collision;
};