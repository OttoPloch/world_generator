#include "game_position.hpp"
#include "../core/game.hpp"

GamePosition::GamePosition(Game* game, sf::Vector2f position, PositionType type) : game(game)
{
    this->position = std::make_shared<sf::Vector2f>(position);
    this->type = std::make_shared<PositionType>(type);
}

GamePosition::GamePosition(const GamePosition& other)
{
    this->game = other.game;
    this->position = other.position;
    this->type = other.type;
}

sf::Vector2f GamePosition::getPosition()
{
    return *position;
}

PositionType GamePosition::getPositionType()
{
    return *type;
}

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

void GamePosition::setPositionType(PositionType newType, bool convertPosition)
{
    PositionType oldType = *type;

    *type = newType;

    if (convertPosition)
    {
        if (game == nullptr)
        {
            std::cout << "ERROR setting GamePosition type, never given Game pointer.\n";
            assert(false);
        }

        if (*type == PositionType::WORLD && oldType == PositionType::SCREEN)
        {
            *position += game->getScene()->getCamera()->getTopLeft();
        }
        else if (*type == PositionType::SCREEN && oldType == PositionType::WORLD)
        {
            *position -= game->getScene()->getCamera()->getTopLeft();
        }
    }
}

sf::Vector2f GamePosition::convertPosition(PositionType returnType)
{
    if (returnType == PositionType::WORLD && *type == PositionType::SCREEN)
    {    
        return *position + game->getScene()->getCamera()->getTopLeft();
    }
    else if (returnType == PositionType::SCREEN && *type == PositionType::WORLD)
    {
        return *position - game->getScene()->getCamera()->getTopLeft();
    }
    
    return *position;
}