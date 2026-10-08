#pragma once

#include "ui_position.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <vector>

class Game;
class Window;

class UIElement
{
public:
    UIElement(Game* p_game, unsigned int p_ID, std::string p_data, UIElement* p_parent = nullptr);

    void setData(std::string p_data);

    void updateVisuals();
    
    UIPosition getUIPosition();
    
    sf::FloatRect getGlobalBounds();

    unsigned int getID();

    UIElement* getChildByIDPath(std::string p_childIDPath);

    void setParent(UIElement* p_parent);

    UIElement* getParent();

    void addChild(UIElement* p_child);

    void removeChild(UIElement* p_child);

    virtual sf::Vector2f getSize();

    virtual void update();

    virtual void draw(bool p_debug);

    virtual ~UIElement();

    Game* m_game;
    Window* m_window;
protected:
    UIElement(Game* p_game, unsigned int p_ID, UIElement* p_parent = nullptr);

    virtual void processDataCommand_Child(std::string p_key, std::string p_value);

    virtual void updateVisuals_Child();

    UIPosition m_position;
    sf::Vector2f m_globalPosition;

    std::vector<UIElement*> m_children;

    const unsigned int m_ID;
private:
    void processDataCommand(std::string p_command);

    sf::Vector2f calculateGlobalPosition();

    void setUIPositionFromString(std::string p_positionData);

    UIElement* m_parent;
};