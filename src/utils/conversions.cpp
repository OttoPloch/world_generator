#include "conversions.hpp"
#include <cmath>

sf::Vector2f toV2F(sf::Vector2i vector)
{
    return {
        static_cast<float>(vector.x),
        static_cast<float>(vector.y)
    };
}

sf::Vector2f toV2F(int x, int y)
{
    return {
        static_cast<float>(x),
        static_cast<float>(y)
    };
}

sf::Vector2f toV2F(sf::Vector2u vector)
{
    return {
        static_cast<float>(vector.x),
        static_cast<float>(vector.y)
    };
}

sf::Vector2f toV2F(unsigned int x, unsigned int y)
{
    return {
        static_cast<float>(x),
        static_cast<float>(y)
    };
}

sf::Vector2i toV2I(sf::Vector2f vector)
{
    return {
        static_cast<int>(vector.x),
        static_cast<int>(vector.y)
    };
}

sf::Vector2i toV2I(float x, float y)
{
    return {
        static_cast<int>(x),
        static_cast<int>(y)
    };
}

sf::Vector2i toV2I(sf::Vector2u vector)
{
    return {
        static_cast<int>(vector.x),
        static_cast<int>(vector.y)
    };
}

sf::Vector2i toV2I(unsigned int x, unsigned int y)
{
    return {
        static_cast<int>(x),
        static_cast<int>(y)
    };
}

double toDouble(float value)
{
    return static_cast<double>(value);
}

double toDouble(int value)
{
    return static_cast<double>(value);
}

double toDouble(unsigned int value)
{
    return static_cast<double>(value);
}

float toFloat(double value)
{
    return static_cast<float>(value);
}

float toFloat(int value)
{
    return static_cast<float>(value);
}

float toFloat(unsigned int value)
{
    return static_cast<float>(value);
}

int toInt(double value)
{
    return static_cast<int>(value);
}

int toInt(float value)
{
    return static_cast<int>(value);
}

int toInt(unsigned int value)
{
    return static_cast<int>(value);
}

unsigned int toUnsignedInt(double value)
{
    if (value >= 0)
    {
        return static_cast<unsigned int>(value);
    }
    
    return 0;
}

unsigned int toUnsignedInt(float value)
{
    if (value >= 0)
    {
        return static_cast<unsigned int>(value);
    }
    
    return 0;
}

unsigned int toUnsignedInt(int value)
{
    if (value >= 0)
    {
        return static_cast<unsigned int>(value);
    }
    
    return 0;
}

float deg2Rad(float degrees)
{
    return degrees * M_PI / 180;
}

float rad2Deg(float radians)
{
    return radians / M_PI * 180;
}