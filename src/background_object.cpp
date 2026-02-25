#include "background_object.hpp"

BackgroundObject::BackgroundObject() {}

BackgroundObject::BackgroundObject(sf::Vector2f center, sf::Vector2f size, sf::Vector2f texCoords, sf::Vector2f texCoordDimensions)
{
    this->center = center;
    this->size = size;
    this->texCoords = texCoords;
    this->texCoordDimensions = texCoordDimensions;
}
