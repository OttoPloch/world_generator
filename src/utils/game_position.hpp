#pragma once

#include "../core/common.hpp"

enum class PositionType
{
    WORLD,
    SCREEN,

    COUNT
};

class GamePosition
{
public:
    GamePosition(Game* game, sf::Vector2f position, PositionType type);

    GamePosition(const GamePosition& other);

    PositionType getPositionType();

    void setPosition(sf::Vector2f newPosition);

    void setPosition(char direction, float value);
    
    void changePosition(sf::Vector2f amount);

    void setPositionType(PositionType newType, bool convertPosition = true);

    // return this object's position as a desired position type.
    sf::Vector2f getPosition(PositionType returnType);

    // static function for general conversions.
    static sf::Vector2f convertPosition(Game* game, sf::Vector2f position, PositionType from, PositionType to);
private:
    Game* game;

    std::shared_ptr<sf::Vector2f> position;

    std::shared_ptr<PositionType> type;
};