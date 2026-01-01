#pragma once

#include "common.hpp"

bool getKey(sf::Keyboard::Key key);

// enter key in all caps
bool getKey(std::string key);

// enter key in all caps
bool getButton(std::string key);

bool getControl(std::string key);

sf::Vector2f getMovement();