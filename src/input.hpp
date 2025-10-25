#pragma once

#include <SFML/Graphics.hpp>

#include "common.hpp"

bool getKey(sf::Keyboard::Key key);

// all caps
bool getKey(std::string key);

sf::Vector2f getMovement();