#pragma once

#include <SFML/Graphics.hpp>

#include "../core/common.hpp"

class Entity;
class Game;
class UILayer;
class UIElement;
class CollisionRect;

float getDistance(sf::Vector2f vec1, sf::Vector2f vec2);

bool rectRectCollide(sf::FloatRect r1, sf::FloatRect r2, bool positionsAreCenter);

bool rayRectCollide(sf::Vector2f rayOrigin, sf::Vector2f rayDirection, CollisionRect* target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& tHitNear);

bool dynamicRectRectCollide(CollisionRect* in, sf::Vector2f inVel, CollisionRect* target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime);

bool mouseRectCollide(Game* game, sf::Vector2f position, sf::Vector2f size);

template<typename T>
void log(T message, bool newLine = true)
{
    std::cout << message;
    if (newLine) std::cout << '\n';
}

sf::Vector2f UIPositionToScreenCenter(UILayer* uiLayer, UIElement* element, sf::Vector2f pos, int posSet = -1, bool useElementPosition = false);

int getRandInt(int min, int max);

int getRandInt(int seed, int min, int max);

int getRandInt();

bool isOnScreen(Game* game, sf::Vector2f tl, sf::Vector2f size, bool applyView = true);

bool isOnScreen(Game* game, sf::Vector2f point, bool applyView = true);

sf::Vector2i worldToChunkPosition(Game* game, sf::Vector2f position);

sf::Vector2f chunkToWorldPosition(Game* game, sf::Vector2i position);