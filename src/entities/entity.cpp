#include "entity.hpp"

Entity::Entity(int ID, Game* game, sf::Vector2f position) : ID(ID), game(game), position(game, position) {}