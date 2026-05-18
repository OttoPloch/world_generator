#include "game_position.hpp"
#include "../core/game.hpp"

GamePosition::GamePosition(Game* game, sf::Vector2f position) : game(game)
{
    this->position = std::make_shared<sf::Vector2f>(position);
}

GamePosition::GamePosition(const GamePosition& other)
{
    this->game = other.game;
    this->position = other.position;
}

sf::Vector2f GamePosition::getPosition() { return *position; }

void GamePosition::setPosition(sf::Vector2f newPosition)
{
    *position = newPosition;
}

void GamePosition::setPosition(char direction, float amount)
{
    if (direction == 'x')
    {
        position->x += amount;
    }
    else if (direction == 'y')
    {
        position->y += amount;
    }
    else
    {
        std::cout << "pick a direction!\n";
        assert(false);
    }
}

void GamePosition::changePosition(sf::Vector2f amount)
{
    *position += amount;
}