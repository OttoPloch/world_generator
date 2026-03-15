#include "background_object.hpp"
#include <SFML/Graphics/Rect.hpp>

BackgroundObject::BackgroundObject() {}

BackgroundObject::BackgroundObject(sf::FloatRect rect, sf::IntRect texCoords) : rect(rect), texCoords(texCoords) {}