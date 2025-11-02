#include "game_position.hpp"

GamePosition::GamePosition() : position(std::make_shared<sf::Vector2f>()) {}

sf::Vector2f GamePosition::get() { return *position; }

void GamePosition::set(sf::Vector2f value) { *position = value; }

void GamePosition::set(char direction, float value)
{
    if (direction == 'x')
    {
        (*position).x = value;
    }
    else if (direction == 'y')
    {
        (*position).y = value;
    }
    else
    {
        std::cout << "pick a direction!\n";
        assert(false);
    }
}

void GamePosition::change(sf::Vector2f amount) { *position += amount; }

void GamePosition::change(char direction, float amount)
{
    if (direction == 'x')
    {
        (*position).x += amount;
    }
    else if (direction == 'y')
    {
        (*position).y += amount;
    }
    else
    {
        std::cout << "pick a direction!\n";
        assert(false);
    }
}