#pragma once

#include "../core/common.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>

class Entity;
class Game;
class UILayer;
class UIElement;

float getDistance(sf::Vector2f vec1, sf::Vector2f vec2);

bool rectRectCollide(sf::FloatRect r1, sf::FloatRect r2, bool positionsAreCenter);

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