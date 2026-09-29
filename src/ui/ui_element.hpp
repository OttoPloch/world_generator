#pragma once

#include <SFML/System/Vector2.hpp>

class Game;
class Window;

class UIElement
{
public:
    UIElement(Game* p_game, sf::Vector2f p_position);

    sf::Vector2f getPosition();

    void setPosition(sf::Vector2f p_newPosition);

    virtual void update();

    virtual void draw();

    virtual ~UIElement();
protected:
    Game* m_game;
    Window* m_window;

    sf::Vector2f m_position;
};