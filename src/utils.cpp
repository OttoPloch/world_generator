#include "utils.hpp"
#include "entity.hpp"
#include "game.hpp"
#include "ui_layer.hpp"
#include "ui_element.hpp"

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
    std::cout << message;

    if (newLine) std::cout << '\n';
}

void log(sf::Vector2f value, bool newLine)
{
    std::cout << value.x << ", " << value.y;
    
    if (newLine) std::cout << '\n';
}

void log(sf::Vector2i value, bool newLine)
{
    std::cout << value.x << ", " << value.y;
    
    if (newLine) std::cout << '\n';
}

sf::Vector2f UIPositionToScreenCenter(UILayer* uiLayer, UIElement* element, sf::Vector2f pos, int posSet, bool useElementPosition)
{
    int set = posSet;
    if (posSet == -1) set = element->getPosSet();
    
    sf::Vector2f position = pos;
    if (useElementPosition) position = element->getPosition();

    UIElement* parent = element->getParent();
    sf::Vector2f size = element->getSize();

    float posX = 0.f;

    switch(set)
    {
        case 0:
        case 2:
        case 6:
            (parent) ? posX = parent->left() + position.x + size.x / 2.f : posX = position.x + size.x / 2.f;
            break;
        case 1:
        case 3:
        case 7:
            (parent) ? posX = parent->right() + position.x - size.x / 2.f : posX = uiLayer->getViewSize().x + position.x - size.x / 2.f;
            break;
        case 9:
            (parent) ? posX = parent->left() + position.x : posX = position.x;
            break;
        default:
            (parent) ? posX = parent->getScreenCenter().x + position.x : posX = (uiLayer->getViewSize().x / 2.f) + position.x;
            break;
    }

    float posY = 0.f;

    switch(set)
    {
        case 0:
        case 1:
        case 5:
            (parent) ? posY = parent->top() + position.y + size.y / 2.f: posY = position.y + size.y / 2.f;
            break;
        case 2:
        case 3:
        case 8:
            (parent) ? posY = parent->bottom() + position.y - size.y / 2.f : posY = uiLayer->getViewSize().y + position.y - size.y / 2.f;
            break;
        case 9:
            (parent) ? posY = parent->top() + position.y : posY = position.y;
            break;
        default:
            (parent) ? posY = parent->getScreenCenter().y + position.y : posY = (uiLayer->getViewSize().y / 2.f) + position.y;
            break;
    }

    return {posX, posY};
}