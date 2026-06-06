#include "background_object.hpp"
#include <SFML/Graphics/Rect.hpp>

BackgroundObject::BackgroundObject() {}

BackgroundObject::BackgroundObject(sf::FloatRect rect, sf::FloatRect texCoords) : rect(rect), texCoords(texCoords), bottom(rect.position.y + rect.size.y) {}