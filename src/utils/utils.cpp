#include "utils.hpp"
#include "../core/game.hpp"
#include "../entities/entity.hpp"
#include "../ui/ui_layer.hpp"
#include "../ui/ui_element.hpp"
#include "../entities/collision_rect.hpp"
#include "game_position.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdint>
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


bool mouseRectCollide(Game* game, sf::Vector2f position, sf::Vector2f size, bool useWorldMousePos)
{
    sf::Vector2f windowPos = game->getInput()->cursor->getGameCursorPosition();
    sf::Vector2f mousePos = windowPos;

    if (useWorldMousePos)
    {
        mousePos = game->getInput()->cursor->getGameCursorCoords();
    }

    return pointRectCollide(mousePos, {position, size});
}

bool pointRectCollide(sf::Vector2f point, sf::FloatRect rect)
{
    if (rect.position.x < point.x && rect.position.x + rect.size.x > point.x && rect.position.y < point.y && rect.position.y + rect.size.y > point.y)
    {
        return true;
    }

    return false;
}

void log(sf::Vector2f msg, bool newLine)
{
    std::cout << msg.x << ", " << msg.y;

    if (newLine) std::cout << '\n';
}

void log(sf::Vector2i msg, bool newLine)
{
    log(static_cast<sf::Vector2f>(msg), newLine);
}

void log(sf::Vector2u msg, bool newLine)
{
    log(static_cast<sf::Vector2f>(msg), newLine);
}

bool isOnScreen(Game* game, sf::Vector2f tl, sf::Vector2f size, bool useCameraView)
{
    Camera* camera = game->getScene()->getCamera();
    sf::Vector2f cameraTopLeft = camera->getTopLeft();
    sf::Vector2f cameraSize = camera->getView().getSize();

    float left = tl.x;
    float right = tl.x + size.x;
    float top = tl.y;
    float bottom = tl.y + size.y;

    if (useCameraView)
    {
        if (right >= cameraTopLeft.x)
        {
            if (left <= cameraTopLeft.x + cameraSize.x)
            {
                if (bottom >= cameraTopLeft.y)
                {
                    if (top <= cameraTopLeft.y + cameraSize.y)
                    {
                        return true;
                    }
                }
            }
        }
    }
    else
    {
        sf::Vector2f UIViewSize = game->getScene()->getUILayer()->getUIView().getSize();

        if (right >= 0)
        {
            if (left <= UIViewSize.x)
            {
                if (bottom >= 0)
                {
                    if (top <= UIViewSize.y)
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool isOnScreen(Game* game, sf::Vector2f point, bool useCameraView)
{
    return isOnScreen(game, point, {0, 0}, useCameraView);
}

sf::Vector2i worldToChunkPosition(Game* game, sf::Vector2f position)
{
    float chunkLength = game->getSettings()->tile_size * game->getSettings()->chunk_size;

    sf::Vector2i pos(std::floor(position.x / chunkLength), std::floor(position.y / chunkLength));
    pos += game->getScene()->getWorldChunkOrigin();

    return pos;
}

sf::Vector2f chunkToWorldPosition(Game* game, sf::Vector2i position)
{
    float chunkLength = game->getSettings()->tile_size * toFloat(game->getSettings()->chunk_size);
    sf::Vector2i worldChunkOrigin = game->getScene()->getWorldChunkOrigin();

    sf::Vector2f pos((position.x - worldChunkOrigin.x) * chunkLength, (position.y - worldChunkOrigin.y) * chunkLength);

    return pos;
}

sf::Vector2i worldToTilePosition(Game* game, sf::Vector2f position, bool localPos, bool applyWorldOrigin)
{
    float tileSize = game->getSettings()->tile_size;

    sf::Vector2i pos(toInt(std::floor(position.x / tileSize)), toInt(std::floor(position.y / tileSize)));

    if (localPos)
    {
        // dont need to worry about applyWorldOrigin if localPos is true, because we just want the remainder
        // of the global tile position / chunk size.
        
        int chunkSize = game->getSettings()->chunk_size;

        pos.x %= chunkSize;
        pos.y %= chunkSize;
    }
    else if (applyWorldOrigin)
    {
        int chunkSize = game->getSettings()->chunk_size;
        sf::Vector2i worldChunkOrigin = game->getScene()->getWorldChunkOrigin();

        pos += sf::Vector2i(worldChunkOrigin.x * chunkSize, worldChunkOrigin.y * chunkSize);
    }

    return pos;
}

sf::Vector2f tileToWorldPosition(Game* game, sf::Vector2i position, bool applyWorldOrigin)
{
    float tileSize = game->getSettings()->tile_size;

    sf::Vector2f pos(position.x * tileSize, position.y * tileSize);

    if (applyWorldOrigin)
    {
        float chunkLength = tileSize * game->getSettings()->chunk_size;
        sf::Vector2i worldChunkOrigin = game->getScene()->getWorldChunkOrigin();

        pos -= sf::Vector2f(worldChunkOrigin.x * chunkLength, worldChunkOrigin.y * chunkLength);
    }

    return pos;
}

void printBlameStats(const std::unordered_map<std::string, float> &blame, std::string category)
{
    std::cout << '\n' + category + ":\n";

    float total = 0;
    for (auto b : blame) total += b.second;

    int messageLength = 0;
    for (auto b : blame)
    {
        std::string message = b.first + ": ";

        if (message.size() > messageLength) messageLength = message.size();
    }

    for (int i = 0; i < messageLength + 20; i++) std::cout << '=';
    std::cout << '\n';

    for (auto b : blame)
    {
        std::string message = b.first + ": ";

        int remaining = messageLength - message.size();

        for (int i = 0; i < remaining; i++) message += ' ';

        message += std::to_string(b.second) + ' ' + std::to_string(100 * b.second / total) + '%';

        std::cout << message << '\n';
    }

    std::cout << "TOTAL: " << total << '\n';
}

float roundToMultiple(float numToRound, float multiple)
{
    float remainder = std::fmod(numToRound, multiple);

    float roundDown = numToRound - remainder;
    float roundUp = numToRound - remainder + multiple;

    if (std::abs(numToRound - roundDown) < std::abs(numToRound - roundUp))
    {
        return roundDown;
    }
    else
    {
        return roundUp;
    }
}

sf::FloatRect getRandomTextureAtlasChoice(Game* game, TextureAtlas* atlas)
{
    // setting tex coords from the options given in the atlas
    std::vector<sf::FloatRect> options;            
    for (auto t : atlas->itemTexCoords)
    {
        options.emplace_back(t.second);
    }

    return options[game->random.getRandInt(0, options.size() - 1)];
}

float getTileScale(Game* game)
{
    return game->getSettings()->tile_size / game->getAssetManager()->getTextureAtlas("tiles_better")->tileSize;
}

float getAngle(sf::Vector2f vec1, sf::Vector2f vec2)
{
    return std::fmod(std::atan2(vec2.x - vec1.x, vec2.y - vec1.y) + 3 * M_PI / 2, 2 * M_PI);
}