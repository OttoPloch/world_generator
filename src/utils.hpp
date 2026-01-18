#pragma once

#include "common.hpp"

class Entity;
class Game;

float getDistance(sf::Vector2f vec1, sf::Vector2f vec2);

int partition(std::vector<Entity*>* vec, int low, int high);

void sortEntitiesByY(std::vector<Entity*>* vec, int low, int high);

bool mouseRectCollide(Game* game, sf::Vector2f position, sf::Vector2f size);