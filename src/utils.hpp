#pragma once

#include "common.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>

class Entity;
class Game;
class UILayer;
class UIElement;

float getDistance(sf::Vector2f vec1, sf::Vector2f vec2);

bool mouseRectCollide(Game* game, sf::Vector2f position, sf::Vector2f size);

void log(std::string message, bool newLine = true);

void log(sf::Vector2f value, bool newLine = false);

void log(sf::Vector2i value, bool newLine = false);

void log(int value, bool newLine = true);

sf::Vector2f UIPositionToScreenCenter(UILayer* uiLayer, UIElement* element, sf::Vector2f pos, int posSet = -1, bool useElementPosition = false);

int getRandInt(int min, int max);

int getRandInt(int seed, int min, int max);

int getRandInt();

bool isOnScreen(Game* game, sf::Vector2f tl, sf::Vector2f size, bool applyView = true);

bool isOnScreen(Game* game, sf::Vector2f point, bool applyView = true);

sf::Vector2i worldToChunkPosition(Game* game, sf::Vector2f position);

sf::Vector2f chunkToWorldPosition(Game* game, sf::Vector2i position);