#pragma once

#include "common.hpp"
#include "game_position.hpp"

#include <array>

class CollisionRect
{
public:
    CollisionRect();

    void init(GamePosition position, sf::Vector2f offset, sf::Vector2f size, std::string colliderName, int rectType, std::vector<std::string> blacklist);

    void updatePosition();

    sf::Vector2f center();
    
    sf::Vector2f getSize();
    
    void setSize(sf::Vector2f size);

    // left, right, top, bottom
    std::array<float, 4> getBorders();

    float left();

    float right();

    float top();

    float bottom();

    float length();
    
    float halfLength();
    
    float width();

    float halfWidth();

    void setBasePosition(sf::Vector2f newPosition);

    void setOffsetPosition(sf::Vector2f newPosition);

    void setOffset(sf::Vector2f newOffset);

    void setLeft(float value);

    void setRight(float value);

    void setTop(float value);

    void setBottom(float value);

    void setToDefault();

    std::string getColliderName();

    int getType();

    void setBlacklist(std::vector<std::string> newList);
    
    void addToBlacklist(std::string newEntry);
    
    bool searchBlacklist(std::string entry);
    
    sf::Vector2f lastPosition;
private:
    GamePosition position;

    sf::Vector2f offset;

    sf::Vector2f defaultOffset;

    sf::Vector2f size;

    sf::Vector2f defaultSize;
    
    sf::Vector2f offsetPosition;

    std::string colliderName;

    std::vector<std::string> blacklist;

    int rectType;
};