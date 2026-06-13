#include "image_component.hpp"
#include "../ui_element.hpp"
#include "../../core/game.hpp"

ImageComponent::ImageComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, sf::Texture* texture, sf::Vector2f size, bool sizeIsScale, Animation* animation, AnimationSet* animSet, bool usingTexCoords, sf::IntRect texCoords, float animSpeedMult) : UIComponent(game, myElement, position, identifier, sortIndex)
{
    // position is initialized to 0, 0; gets synced in updateVisuals()
    sprite = Sprite(GamePosition(game, {0, 0}), texture, size, sizeIsScale, usingTexCoords, texCoords, animSpeedMult);

    if (animation) sprite.animation = std::make_unique<Animation>(*animation);
    if (animSet) sprite.animSet = std::make_unique<AnimationSet>(*animSet);
}

sf::FloatRect ImageComponent::getLocalBounds()
{
    return {position.position + originOffset + anchorOffset, sprite.getSize()};
}

void ImageComponent::resize(sf::Vector2f newSize)
{
    sprite.resize(newSize, false);

    updateVisuals();
}

void ImageComponent::updateVisuals()
{
    originOffset = UIPosition::getOriginOffset(position, sprite.getSize());
    anchorOffset = UIPosition::getAnchorOffset(position, myElement->getLocalBoundsUpToComponent(sortIndex));

    sprite.getPositionVar().setPosition(myElement->effectiveTopLeft + position.position + originOffset + anchorOffset + (sprite.getSize() / 2.f));
    sprite.syncPos();
}

void ImageComponent::update(float dt)
{
    updateState();

    sprite.update(dt);
}

void ImageComponent::draw(bool debug)
{
    sprite.draw(game->getWindow()->getWindow());
}