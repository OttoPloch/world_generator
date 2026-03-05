#pragma once

#include "common.hpp"
#include "world_position.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

class Sprite
{
public:
    Sprite();

    Sprite(WorldPosition position, sf::Texture* texture, sf::Vector2f size = {1.f, 1.f}, bool sizeIsScale = true, bool usingTexCoords = false, sf::IntRect texCoords = sf::IntRect({0, 0}, {0, 0}));

    sf::Vector2f getPosition();
    sf::Vector2f getSize();

    float left();
    float right();
    float top();
    float bottom();

    void draw(sf::RenderWindow& window);

    std::unique_ptr<sf::Sprite> sprite;
private:
    WorldPosition position;

    sf::Vector2f size;

    sf::Texture* texture;

    sf::FloatRect texCoords;
};
