#pragma once

#include <SFML/Graphics.hpp>

sf::Vector2f toV2F(sf::Vector2i vector);

sf::Vector2f toV2F(int x, int y);

sf::Vector2f toV2F(sf::Vector2u vector);

sf::Vector2f toV2F(unsigned int x, unsigned int y);

sf::Vector2i toV2I(sf::Vector2f vector);

sf::Vector2i toV2I(float x, float y);

sf::Vector2i toV2I(sf::Vector2u vector);

sf::Vector2i toV2I(unsigned int x, unsigned int y);

float toFloat(int value);

float toFloat(unsigned int value);

int toInt(float value);

int toInt(unsigned int value);