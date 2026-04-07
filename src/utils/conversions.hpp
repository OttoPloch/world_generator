#pragma once

#include <SFML/Graphics.hpp>

// i think this is what templates are for

sf::Vector2f toV2F(sf::Vector2i vector);

sf::Vector2f toV2F(int x, int y);

sf::Vector2f toV2F(sf::Vector2u vector);

sf::Vector2f toV2F(unsigned int x, unsigned int y);

sf::Vector2i toV2I(sf::Vector2f vector);

sf::Vector2i toV2I(float x, float y);

sf::Vector2i toV2I(sf::Vector2u vector);

sf::Vector2i toV2I(unsigned int x, unsigned int y);

double toDouble(float value);

double toDouble(int value);

double toDouble(unsigned int value);

float toFloat(double value);

float toFloat(int value);

float toFloat(unsigned int value);

int toInt(double value);

int toInt(float value);

int toInt(unsigned int value);

unsigned int toUnsignedInt(double value);

unsigned int toUnsignedInt(float value);

unsigned int toUnsignedInt(int value);

float deg2Rad(float degrees);

float rad2Deg(float radians);