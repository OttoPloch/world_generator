#pragma once

#include <map>
#include <memory>
#include <SFML/Graphics/View.hpp>
#include "ui_element.hpp"

class Game;
class Camera;

class UILayer
{
public:
    UILayer(Game* p_game, Camera* p_camera);

    void addElement(std::unique_ptr<UIElement> p_newElement);

    void updateVisuals();

    void update(float p_dt);

    void tick();

    void draw(bool p_debug);
private:
    unsigned int getNewID();

    void setUIViewSize();

    unsigned int m_IDCounter;

    Game* m_game;
    Camera* m_camera;

    sf::View UIView;

    std::map<unsigned int, std::unique_ptr<UIElement>> m_elements;
};