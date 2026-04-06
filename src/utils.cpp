#include "utils.hpp"
#include "entity.hpp"
#include "game.hpp"
#include "ui_layer.hpp"
#include "ui_element.hpp"
#include <cstdlib>
#include <random>

float getDistance(sf::Vector2f vec1, sf::Vector2f vec2)
{
    return std::fabs(sqrt(pow(vec2.x - vec1.x, 2) + pow(vec2.y - vec1.y, 2)));
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

int getRandInt(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);

    return dist(gen);
}

int getRandInt(int seed, int min, int max)
{
    srand(seed);

    int diff = max - min + 1;

    return rand() % diff + min;
}

int getRandInt()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist;

    return dist(gen);
}

bool isOnScreen(Game* game, sf::Vector2f tl, sf::Vector2f size, bool applyView)
{
    sf::RenderWindow& window = game->getWindow()->getWindow();
    sf::View view = window.getView();

    float top = tl.y;
    float bottom = tl.y + size.y;
    float left = tl.x;
    float right = tl.x + size.x;

    if (applyView)
    {
        if (right >= view.getCenter().x - view.getSize().x / 2.f)
        {
            if (left <= view.getCenter().x + view.getSize().x / 2.f)
            {
                if (bottom >= view.getCenter().y - view.getSize().y / 2.f)
                {
                    if (top <= view.getCenter().y + view.getSize().y / 2.f)
                    {
                        return true;
                    }
                }
            }
        }
    }
    else
    {
        if (right >= 0)
        {
            if (left <= window.getSize().x)
            {
                if (bottom >= 0)
                {
                    if (top <= window.getSize().y)
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool isOnScreen(Game* game, sf::Vector2f point, bool applyView)
{
    sf::RenderWindow& window = game->getWindow()->getWindow();
    sf::View view = window.getView();

    if (applyView)
    {
        if (point.x >= view.getCenter().x - view.getSize().x / 2.f)
        {
            if (point.x <= view.getCenter().x + view.getSize().x / 2.f)
            {
                if (point.y >= view.getCenter().y - view.getSize().y / 2.f)
                {
                    if (point.y <= view.getCenter().y + view.getSize().y / 2.f)
                    {
                        return true;
                    }
                }
            }
        }
    }
    else
    {
        if (point.x >= 0)
        {
            if (point.x <= window.getSize().x)
            {
                if (point.y >= 0)
                {
                    if (point.y <= window.getSize().y)
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

sf::Vector2i worldToChunkPosition(Game* game, sf::Vector2f position)
{
    float chunkSize = game->getSettings()->tile_size * toFloat(game->getSettings()->chunk_size);

    return {toInt(std::floor(position.x / chunkSize)), toInt(std::floor(position.y / chunkSize))};
}

sf::Vector2f chunkToWorldPosition(Game* game, sf::Vector2i position)
{
    float chunkSize = game->getSettings()->tile_size * toFloat(game->getSettings()->chunk_size);

    return {position.x * chunkSize, position.y * chunkSize};
}
