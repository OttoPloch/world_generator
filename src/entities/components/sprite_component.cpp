#include "sprite_component.hpp"
#include "../../graphics/sprite.hpp"
#include "../entity.hpp"
#include <SFML/Graphics/Rect.hpp>

SpriteComponent::SpriteComponent(Entity* myEntity, sf::Texture* texture, sf::Vector2f size, bool sizeIsScale, Animation* animation, AnimationSet* animSet, bool usingTexCoords, sf::IntRect texCoords, float animSpeedMult) : EntityComponent(myEntity), sprite(myEntity->position, texture, size, sizeIsScale, usingTexCoords, texCoords, animSpeedMult)
{
    if (animation) sprite.animation = std::make_unique<Animation>(*animation);
    if (animSet) sprite.animSet = std::make_unique<AnimationSet>(*animSet);
}