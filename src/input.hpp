#pragma once

#include "common.hpp"

#include <map>
#include <vector>

class Game;

class Input
{
public:
    Input();

    void init(Game* game);

    // enter key in all caps
    bool getKey(std::string key);
    
    // enter key in all caps
    bool getButton(std::string key);
    
    bool getControl(std::string key);

    bool leftClick();

    sf::Vector2f getMovement();

    void tick();

    void update();
private:
    Game* game;

    std::vector<std::string> keys;

    std::unordered_map<std::string, int> stringToKey;

    std::unordered_map<int, std::string> keyToString;

    std::vector<std::string> buttons;

    std::unordered_map<std::string, int> stringToButton;

    std::unordered_map<int, std::string> buttonToString;

    std::vector<std::pair<std::string, std::pair<std::string, std::string>>> controls;

    std::unordered_map<std::string, bool> keysPressedThisFrame;
    std::unordered_map<std::string, bool> keysPressedLastFrame;

    std::unordered_map<std::string, bool> buttonsPressedThisFrame;
    std::unordered_map<std::string, bool> buttonsPressedLastFrame;
    
    std::unordered_map<std::string, bool> controlsPressedThisFrame;
    std::unordered_map<std::string, bool> controlsPressedLastFrame;

    bool leftClickLastTick;
};