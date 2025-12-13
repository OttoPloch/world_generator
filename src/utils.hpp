#pragma once

#include "common.hpp"

float getDistance(sf::Vector2f vec1, sf::Vector2f vec2);

float getAngleRadians(float y, float x);

float getAngleRadians(sf::Vector2f vec1, sf::Vector2f vec2);

float roundToPlace(float value, int decimalPlaces);