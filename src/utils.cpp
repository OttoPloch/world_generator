#include "utils.hpp"
#include "entity.hpp"
#include "game.hpp"

float getDistance(sf::Vector2f vec1, sf::Vector2f vec2)
{
    return abs(sqrt(pow(vec2.x - vec1.x, 2) + pow(vec2.y - vec1.y, 2)));
}

int partition(std::vector<Entity*>* vec, int low, int high)
{
    int pivot = (*vec)[high]->getSprite()->getBottom();

    int i = low - 1;

    for (int j = low; j <= high - 1; j++)
    {
        if ((*vec)[j]->getSprite()->getBottom() <= pivot)
        {
            i++;
            std::swap((*vec)[i], (*vec)[j]);
        }
    }

    std::swap((*vec)[i + 1], (*vec)[high]);

    return (i + 1);
}

void sortEntitiesByY(std::vector<Entity*>* vec, int low, int high)
{
    if (low < high)
    {
        int pi = partition(vec, low, high);
        
        sortEntitiesByY(vec, low, pi - 1);
        sortEntitiesByY(vec, pi + 1, high);
    }
}

bool mouseRectCollide(Game* game, sf::Vector2f position, sf::Vector2f size)
{
    sf::Vector2f mouse = toV2F(sf::Mouse::getPosition(game->getWindow()->getWindow()).x, sf::Mouse::getPosition(game->getWindow()->getWindow()).y);

    if (position.x < mouse.x && position.x + size.x > mouse.x && position.y < mouse.y && position.y + size.y > mouse.y)
    {
        return true;
    }

    return false;
}

void log(std::string message, bool newLine)
{
    if (newLine)
    {
        std::cout << message << '\n';
    }
    else
    {
        std::cout << message;
    }
}