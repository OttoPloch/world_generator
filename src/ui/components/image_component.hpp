#pragma once

#include "ui_component.hpp"
#include "../../graphics/sprite.hpp"

struct ImageComponent : public UIComponent
{
    ImageComponent(Game* game, UIElement* myElement, UIPosition position, std::string identifier, int sortIndex, sf::Texture* texture, sf::Vector2f size = {1.f, 1.f}, bool sizeIsScale = true, Animation* animation = nullptr, AnimationSet* animSet = nullptr, bool usingTexCoords = false, sf::IntRect texCoords = sf::IntRect({0, 0}, {0, 0}), float animSpeedMult = 1.f, bool repeatAnimation = true);

    sf::FloatRect getLocalBounds() override;

    void resize(sf::Vector2f newSize) override;

    void updateVisuals() override;

    void update(float dt) override;

    void draw(bool debug) override;
private:
    Sprite sprite;
};