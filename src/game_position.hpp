#pragma once

#include <memory>

#include "common.hpp"

class GamePosition
{
public:
    GamePosition();

    sf::Vector2f get();

    void set(sf::Vector2f value);

    void set(char direction, float value);

    void change(sf::Vector2f amount);

    void change(char direction, float amount);
private:
    std::shared_ptr<sf::Vector2f> position;
};