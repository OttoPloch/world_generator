#pragma once

#include <SFML/Graphics.hpp>

#include "../core/common.hpp"

class Entity;
class Game;
class UILayer;
class UIElement;
class CollisionRect;
class GamePosition;

float getDistance(sf::Vector2f vec1, sf::Vector2f vec2);

bool rectRectCollide(sf::FloatRect r1, sf::FloatRect r2, bool positionsAreCenter);

bool rayRectCollide(sf::Vector2f rayOrigin, sf::Vector2f rayDirection, CollisionRect* target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& tHitNear);

bool dynamicRectRectCollide(CollisionRect* in, sf::Vector2f inVel, CollisionRect* target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime);

bool mouseRectCollide(Game* game, sf::Vector2f position, sf::Vector2f size);

bool pointRectCollide(sf::Vector2f point, sf::FloatRect rect);

template<typename T>
void log(T message, bool newLine = true)
{
    std::cout << message;
    if (newLine) std::cout << '\n';
}

// if !useCameraView, then the current applied view of the window will be used.
bool isOnScreen(Game* game, sf::Vector2f tl, sf::Vector2f size, bool useCameraView = true);

bool isOnScreen(Game* game, GamePosition position, sf::Vector2f size);

// if !useCameraView, then the current applied view of the window will be used.
bool isOnScreen(Game* game, sf::FloatRect rect, bool useCameraView = true);

// if !useCameraView, then the current applied view of the window will be used.
bool isOnScreen(Game* game, sf::Vector2f point, bool useCameraView = true);

bool isOnScreen(Game* game, GamePosition position);

sf::Vector2i worldToChunkPosition(Game* game, sf::Vector2f position);

sf::Vector2f chunkToWorldPosition(Game* game, sf::Vector2i position);

// returns a global tile position, not a position within a chunk
sf::Vector2i worldToTilePosition(Game* game, sf::Vector2f position);

// assumes the position given is a global position, not within a chunk
sf::Vector2f tileToWorldPosition(Game* game, sf::Vector2i position);

void printBlameStats(const std::unordered_map<std::string, float>& blame, std::string category);

float roundToMultiple(float numToRound, float multiple);