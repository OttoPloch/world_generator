#include "utils.hpp"
#include "entity.hpp"

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