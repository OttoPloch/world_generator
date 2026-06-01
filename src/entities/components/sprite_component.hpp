#pragma once

#include "entity_component.hpp"
#include "../../graphics/sprite.hpp"
#include <SFML/Graphics/Rect.hpp>

struct SpriteComponent : public EntityComponent
{
    SpriteComponent(Entity* myEntity, sf::Texture* texture, sf::Vector2f size = {1.f, 1.f}, bool sizeIsScale = true, Animation* animation = nullptr, AnimationSet* animSet = nullptr, bool usingTexCoords = false, sf::IntRect texCoords = sf::IntRect({0, 0}, {0, 0}), float animSpeedMult = 1.f);

    Sprite sprite;
};