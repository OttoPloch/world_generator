#include "utils.hpp"
#include "../core/game.hpp"
#include "../entities/entity.hpp"
#include "../ui/ui_layer.hpp"
#include "../ui/ui_element.hpp"
#include "../entities/collision_rect.hpp"
#include "game_position.hpp"
#include <cstdlib>
#include <random>

float getDistance(sf::Vector2f vec1, sf::Vector2f vec2)
{
    return std::fabs(sqrt(pow(vec2.x - vec1.x, 2) + pow(vec2.y - vec1.y, 2)));
}

bool rectRectCollide(sf::FloatRect r1, sf::FloatRect r2, bool positionsAreCenter)
{
    if (positionsAreCenter)
    {
        r1.position.x -= r1.size.x / 2.f;
        r1.position.y -= r1.size.y / 2.f;
        r2.position.x -= r2.size.x / 2.f;
        r2.position.y -= r2.size.y / 2.f;
    }

    return (r1.position.x < r2.position.x + r2.size.x && r1.position.x + r1.size.x > r2.position.x && r1.position.y < r2.position.y + r2.size.y && r1.position.y + r1.size.y > r2.position.y);
}

bool rayRectCollide(sf::Vector2f rayOrigin, sf::Vector2f rayDirection, CollisionRect* target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& tHitNear)
{
    // STOLEN from javidx9

    sf::Vector2f targetPos(target->left(), target->top());

    sf::Vector2f tNear = (targetPos - rayOrigin);
    tNear = {tNear.x / rayDirection.x, tNear.y / rayDirection.y};
    
    sf::Vector2f tFar = (targetPos + (target->size) - rayOrigin);
    tFar = {tFar.x / rayDirection.x, tFar.y / rayDirection.y};

    if (std::isnan(tFar.y) || std::isnan(tFar.x)) return false;
    if (std::isnan(tNear.y) || std::isnan(tNear.x)) return false;


    if (tNear.x > tFar.x) std::swap(tNear.x, tFar.x);
    if (tNear.y > tFar.y) std::swap(tNear.y, tFar.y);

    if (tNear.x > tFar.y || tNear.y > tFar.x) return false;

    tHitNear = std::max(tNear.x, tNear.y);
    float tHitFar = std::min(tFar.x, tFar.y);

    if (tHitFar < 0) return false;

    contactPoint = rayOrigin + tHitNear * rayDirection;

    if (tNear.x > tNear.y)
    {
        if (rayDirection.x < 0) contactNormal = {1, 0};
        else contactNormal = {-1, 0};
    }
    else if (tNear.x < tNear.y)
    {
        if (rayDirection.y < 0) contactNormal = {0, 1};
        else contactNormal = {0, -1};
    }

    return true;
}

bool dynamicRectRectCollide(CollisionRect* in, sf::Vector2f inVel, CollisionRect* target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime)
{
    // STOLEN as well from javidx9

    if (inVel.x == 0 && inVel.y == 0) return false;

    CollisionRect expandedTarget(
        target->position,
        {target->size.x + in->size.x, target->size.y + in->size.y},
        target->type
    );

    if (rayRectCollide(in->position.getPosition(), inVel, &expandedTarget, contactPoint, contactNormal, contactTime))
    {
        if (contactTime <= 1.f) return true;
    }

    return false;
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

// sf::Vector2f UIPositionToScreenCenter(UILayer* uiLayer, UIElement* element, sf::Vector2f pos, int posSet, bool useElementPosition)
// {
//     int set = posSet;
//     if (posSet == -1) set = element->getPosSet();
    
//     sf::Vector2f position = pos;
//     if (useElementPosition) position = element->getPosition();

//     UIElement* parent = element->getParent();
//     sf::Vector2f size = element->getSize();

//     float posX = 0.f;

//     switch(set)
//     {
//         case 0:
//         case 2:
//         case 6:
//             (parent) ? posX = parent->left() + position.x + size.x / 2.f : posX = position.x + size.x / 2.f;
//             break;
//         case 1:
//         case 3:
//         case 7:
//             (parent) ? posX = parent->right() + position.x - size.x / 2.f : posX = uiLayer->getViewSize().x + position.x - size.x / 2.f;
//             break;
//         case 9:
//             (parent) ? posX = parent->left() + position.x : posX = position.x;
//             break;
//         default:
//             (parent) ? posX = parent->getScreenCenter().x + position.x : posX = (uiLayer->getViewSize().x / 2.f) + position.x;
//             break;
//     }

//     float posY = 0.f;

//     switch(set)
//     {
//         case 0:
//         case 1:
//         case 5:
//             (parent) ? posY = parent->top() + position.y + size.y / 2.f: posY = position.y + size.y / 2.f;
//             break;
//         case 2:
//         case 3:
//         case 8:
//             (parent) ? posY = parent->bottom() + position.y - size.y / 2.f : posY = uiLayer->getViewSize().y + position.y - size.y / 2.f;
//             break;
//         case 9:
//             (parent) ? posY = parent->top() + position.y : posY = position.y;
//             break;
//         default:
//             (parent) ? posY = parent->getScreenCenter().y + position.y : posY = (uiLayer->getViewSize().y / 2.f) + position.y;
//             break;
//     }

//     return {posX, posY};
// }

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
        sf::Vector2f viewSize = view.getSize();

        if (right >= 0)
        {
            if (left <= viewSize.x)
            {
                if (bottom >= 0)
                {
                    if (top <= viewSize.y)
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool isOnScreen(Game* game, GamePosition tl, sf::Vector2f size)
{
    sf::RenderWindow& window = game->getWindow()->getWindow();
    sf::View view = window.getView();

    float left = tl.getPosition().x;
    float right = tl.getPosition().x + size.x;
    float top = tl.getPosition().y;
    float bottom = tl.getPosition().y + size.y;

    if (tl.getPositionType() == PositionType::WORLD)
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
    else if (tl.getPositionType() == PositionType::SCREEN)
    {
        sf::Vector2f viewSize = view.getSize();

        if (right >= 0)
        {
            if (left <= viewSize.x)
            {
                if (bottom >= 0)
                {
                    if (top <= viewSize.y)
                    {
                        return true;
                    }
                }
            }
        }
    }
    else
    {
        std::cout << "ERROR in utils, checking isOnScreen for GamePosition with invalid type of " << static_cast<int>(tl.getPositionType()) << '\n';
        assert(false);
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
        sf::Vector2f viewSize = view.getSize();

        if (point.x >= 0)
        {
            if (point.x <= viewSize.x)
            {
                if (point.y >= 0)
                {
                    if (point.y <= viewSize.y)
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool isOnScreen(Game* game, GamePosition position)
{
    sf::Vector2f point = position.getPosition();

    if (position.getPositionType() == PositionType::WORLD)
    {
        sf::RenderWindow& window = game->getWindow()->getWindow();
        sf::View view = window.getView();

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
    else if (position.getPositionType() == PositionType::SCREEN)
    {
        sf::Vector2f viewSize = game->getWindow()->getWindow().getView().getSize();

        if (point.x >= 0)
        {
            if (point.x <= viewSize.x)
            {
                if (point.y >= 0)
                {
                    if (point.y <= viewSize.y)
                    {
                        return true;
                    }
                }
            }
        }
    }
    else
    {
        std::cout << "ERROR in utils, checking isOnScreen for GamePosition with invalid type of " << static_cast<int>(position.getPositionType()) << '\n';
        assert(false);
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

sf::Vector2i worldToTilePosition(Game* game, sf::Vector2f position)
{
    float tileSize = game->getSettings()->tile_size;

    return {toInt(std::floor(position.x / tileSize)), toInt(std::floor(position.y / tileSize))};
}