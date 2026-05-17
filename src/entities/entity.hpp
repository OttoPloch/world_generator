#pragma once

#include "../core/common.hpp"
#include "components/entity_component.hpp"
#include "../utils/game_position.hpp"
#include "../graphics/sprite.hpp"
#include "components/entity_component.hpp"
#include "entity_template.hpp"

#include <SFML/Graphics.hpp>

class Game;

class Entity
{
public:
    Entity();

    Entity(Game* game, int ID, GamePosition position);

    Sprite* spriteInit(sf::Texture* texture, sf::Vector2f size = {1.f, 1.f}, bool sizeIsScale = true, bool usingTexCoords = false, sf::IntRect texCoords = sf::IntRect({0, 0}, {0, 0}), float animSpeedMult = 1.f);

    int getID();

    template<typename T>
    T* getComponent()
    {
        for (auto& c : components)
        {
            if (auto casted = dynamic_cast<T*>(c.get())) return casted;
        }
        
        return nullptr;
    }

    template<typename T, typename... Args>
    T& addComponent(Args&&... args)
    {
        T* comp = new T(std::forward<Args>(args)...);
        components.emplace_back(comp);
        return *comp;
    }

    template<typename T>
    void removeComponent()
    {
        for (auto i = components.begin(); i != components.end();)
        {
            if (auto casted = dynamic_cast<T*>(i->get()))
            {
                i = components.erase(i);
                break;
            }
            else
            {
                ++i;
            }
        }
    }

    void tick();

    void update(float dt);

    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition();

    GamePosition* getPositionVar();

    Sprite* getSprite();
    
    Game* game;
protected:

    int ID;

    GamePosition position;
    sf::Vector2f lastPosition;

    Sprite sprite;

    std::vector<std::unique_ptr<EntityComponent>> components;
};