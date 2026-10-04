#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>

class Game;
class Window;

class UIElement
{
public:
    UIElement(Game* p_game, sf::Vector2f p_position);

    sf::Vector2f getPosition();

    void setPosition(sf::Vector2f p_newPosition);

    void setData(std::string p_data);

    virtual void updateVisuals() = 0;

    virtual void update();

    virtual void draw();

    virtual ~UIElement();
protected:
    void processDataCommand(std::string p_command);

    virtual void processDataCommand_Child(std::string p_key, std::string p_value) = 0;

    Game* m_game;
    Window* m_window;

    sf::Vector2f m_position;
};