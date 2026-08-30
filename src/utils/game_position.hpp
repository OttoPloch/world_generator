#pragma once

#include "../core/common.hpp"

class GamePosition
{
public:
    GamePosition(Game* game, sf::Vector2f position);

    GamePosition(const GamePosition& other);

    sf::Vector2f getPosition();

    void setPosition(sf::Vector2f newPosition);

    void setPosition(char direction, float value);
    
    void changePosition(sf::Vector2f amount);
private:
    Game* game;

    std::shared_ptr<sf::Vector2f> position;
};