#pragma once

#include "common.hpp"
#include "camera.hpp"
#include "asset_manager.hpp"
#include "ui_element.hpp"
#include "ui_background.hpp"
#include "ui_text.hpp"

#include <vector>

class Game;

class UILayer
{
public:
    UILayer();

    void init(Game* game, Camera* camera);

    UIElement* getElement(int ID);
    
    int getNewID();

    sf::Vector2u getScreenSize();

    void reset();
    
    void tick();

    void draw();
private:
    Game* game;

    AssetManager* assetManager;

    Camera* camera;

    int IDCounter;

    sf::View UIView;

    std::vector<UIBackground> bgElements;

    std::vector<UIText> textElements;
};