#include "utils.hpp"

float getDistance(sf::Vector2f vec1, sf::Vector2f vec2)
{
    return abs(sqrt(pow(vec2.x - vec1.x, 2) + pow(vec2.y - vec1.y, 2)));
}

float getAngleRadians(float y, float x)
{
    return atan2(toDouble(y), toDouble(x));
}

float getAngleRadians(sf::Vector2f vec1, sf::Vector2f vec2)
{
    float y = vec2.y - vec1.y;
    float x = vec2.x - vec1.x;

    return getAngleRadians(y, x);
}

float roundToPlace(float value, int decimalPlaces)
{
    return round(toDouble(value) * pow(10, decimalPlaces)) / pow(10, decimalPlaces);
}