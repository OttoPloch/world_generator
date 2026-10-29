#pragma once

#include "ui_position.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <cstdint>
#include <string>
#include <unordered_set>

class Game;
class Window;

class UIElement
{
public:
    UIElement(Game* p_game, unsigned int p_ID, std::string p_data, unsigned int p_parent = UINT32_MAX);

    virtual ~UIElement();

    void setData(std::string p_data);

    std::string retrieveData(std::string p_dataToRetrieve);

    void updateVisuals();
    
    UIPosition getUIPosition();
    
    sf::FloatRect getGlobalBounds();

    unsigned int getID();

    // UIElement* getChildByIDPath(std::string p_childIDPath);

    // leave argument blank to get rid of the parent (just the association, DOES NOT DELETE THE PARENT ELEMENT)
    void setParent(unsigned int p_parent = UINT32_MAX);

    unsigned int getParentID();

    void addChild(unsigned int p_childID);

    void removeChild(unsigned int p_childID);

    // get the bb that contains this element and all its children.
    sf::FloatRect getFamilyBoundingBox();

    // get the size of just this element, not including children.
    virtual sf::Vector2f getSize();

    virtual void update(float p_dt);

    virtual void draw(bool p_debug);

    Game* m_game;
    Window* m_window;

    bool m_visible;
protected:
    UIElement(Game* p_game, unsigned int p_ID, unsigned int p_parent = UINT32_MAX);

    virtual void processDataCommand_Child(std::string p_key, std::string p_value);

    virtual void updateVisuals_Child();

    virtual std::string retrieveData_Child(std::string p_dataToRetrieve);

    UIPosition m_position;
    sf::Vector2f m_globalPosition;

    std::unordered_set<unsigned int> m_children;

    const unsigned int m_ID;
private:
    void processDataCommand(std::string p_command);

    sf::Vector2f calculateGlobalPosition();

    void setUIPositionFromString(std::string p_positionData);

    unsigned int m_parent;
};