#pragma once

#include "common.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>

class Entity;
class Game;
class UILayer;
class UIElement;

float getDistance(sf::Vector2f vec1, sf::Vector2f vec2);

int partition(std::vector<Entity*>* vec, int low, int high);

void sortEntitiesByY(std::vector<Entity*>* vec, int low, int high);

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

// int partition(std::vector<sf::Vertex>* vertices, int low, int high);

// void sortVertices(std::vector<sf::Vertex>* vertices, int low, int high, sf::PrimitiveType type = sf::PrimitiveType::Triangles);