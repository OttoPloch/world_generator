#pragma once

#include "common.hpp"
#include "ui_element.hpp"

class UIButton : public UIElement
{
public:
    UIButton();

    // buttonTextures[0] = default texture
    // buttonTextures[1] = hover texture
    // buttonTextures[2] = click texture
    UIButton(Game* game, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, std::array<sf::Texture*, 3> buttonTextures, std::string parentName = "");

    void init(std::array<sf::Texture*, 3> buttonTextures);

    bool hover();

    bool clicked();

    bool getActive();

    UIButton* getAsButton() override;

    void switchTexture(int index);

    void resize(sf::Vector2f newSize, int posSet = -1) override;

    void tick() override;

    void draw() override;
private:
    std::array<sf::Texture*, 3> buttonTextures;

    std::unique_ptr<sf::Sprite> button;

    bool active;
};