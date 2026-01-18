#pragma once

#include "common.hpp"
#include "camera.hpp"
#include "asset_manager.hpp"
#include "ui_element.hpp"
#include "ui_background.hpp"
#include "ui_text.hpp"
#include "ui_button.hpp"

#include <vector>
#include <map>

class Game;

class UILayer
{
public:
    UILayer();

    void init(Game* game, Camera* camera);

    UIElement* getElement(int ID);

    UIElement* getElement(std::string name);

    int getNewID();

    sf::Vector2u getScreenSize();

    void reset();
    
    void tick();

    void draw();
private:
    Game* game;

    AssetManager* assetManager;

    Camera* camera;

    std::unordered_map<std::string, int> namesToIDs;

    int IDCounter;

    sf::View UIView;

    std::map<int, UIBackground> bgElements;
    
    std::map<int, UIText> textElements;
    
    std::map<int, UIButton> buttonElements;
};