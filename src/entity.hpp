#pragma once

#include "common.hpp"
#include "sprite.hpp"
#include "asset_manager.hpp"
#include "motion_attribute.hpp"
#include "game_position.hpp"
#include "entity_states.hpp"
#include "tile.hpp"

#include <memory>
#include <vector>

class CollisionAttribute;
class Game;

class Entity
{
public:
    Entity();

    Entity(Game* game, int ID, sf::Vector2f position);

    void create(Game* game, int ID, sf::Vector2f position);

    int getID();

    void giveSprite(sf::Texture* texture, sf::Vector2f size = {-1.f, -1.f}, int z = 0, bool centerOrigin = true);
    
    void giveMotion(float mass, bool controlling = false);

    void giveCollision(std::vector<std::unique_ptr<Entity>>* entities, std::string name, int rectType, std::vector<std::string> blacklist = {}, sf::Vector2f offsetFraction = {0, 0}, sf::Vector2f size = {1, 1}, bool sizeIsFraction = true);

    void changeSpriteTexture(sf::Texture* texture);

    void tick(std::vector<std::unique_ptr<Entity>>* entities = nullptr, std::vector<Tile>* tiles = nullptr);

    void update(float dt);

    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition();

    EntityStates* getStates();

    Sprite* getSprite();

    MotionAttribute* getMotion();

    CollisionAttribute* getCollision();
private:
    int ID;

    Game* game;

    GamePosition position;

    float rotation;

    std::unique_ptr<Sprite> sprite;

    std::unique_ptr<MotionAttribute> motion;

    std::unique_ptr<CollisionAttribute> collision;

    EntityStates states;
};