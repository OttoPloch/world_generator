#pragma once

#include "../core/common.hpp"
#include "ui_element.hpp"
#include <SFML/Graphics/Rect.hpp>

class UIButton : public UIElement
{
public:
    UIButton();

    // buttonTextures[0] = default texture
    // buttonTextures[1] = hover texture
    // buttonTextures[2] = click texture
    UIButton(Game* game, UILayer* uiLayer, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, std::array<sf::Texture*, 3> buttonTextures, UIElement* parent = nullptr);
    
    // buttonTextures[0] = default texture
    // buttonTextures[1] = hover texture
    // buttonTextures[2] = click texture
    void init(Game* game, UILayer* uiLayer, std::string name, int ID, unsigned int posSet, sf::Vector2f position, sf::Vector2f size, std::array<sf::Texture*, 3> buttonTextures, UIElement* parent = nullptr);
    
    // buttonTextures[0] = default texture
    // buttonTextures[1] = hover texture
    // buttonTextures[2] = click texture
    void init(std::array<sf::Texture*, 3> buttonTextures);

    sf::FloatRect getBoundingBox() override;

    bool hover();

    bool pressed();

    bool clicked();

    void activate();

    bool getActive();

    UIButton* getAsButton() override;

    void switchTexture(int index);

    void resize(sf::Vector2f newSize, int posSet = -1) override;

    void update() override;

    void draw() override;
private:
    std::array<sf::Texture*, 3> buttonTextures;

    std::unique_ptr<sf::Sprite> button;

    bool active;
};