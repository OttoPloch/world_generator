#pragma once

#include <map>
#include <memory>
#include <SFML/Graphics/View.hpp>
#include "ui_element.hpp"
#include "ui_management_system.hpp"

class Game;
class Camera;

class UILayer
{
public:
    UILayer(Game* p_game, Camera* p_camera);

    unsigned int getNewID();

    sf::Vector2f getUIViewSize();

    template<typename T>
    UIElement* addElement(std::string p_elementData, UIElement* p_parent = nullptr)
    {
        unsigned int l_newID = getNewID();

        std::unique_ptr<T> l_newElement = std::make_unique<T>(m_game, l_newID, p_elementData, p_parent);
        if (!dynamic_cast<UIElement*>(l_newElement.get())) return nullptr;

        m_elements[l_newID] = std::move(l_newElement);

        return m_elements[l_newID].get();
    }
    
    UIElement* getElement(unsigned int ID);

    UIManagementSystem* getUIManagementSystem();

    void updateVisuals();

    void update(float p_dt);

    void tick();

    void draw(bool p_debug);
private:
    void setUIViewSize();

    unsigned int m_IDCounter;

    Game* m_game;
    Camera* m_camera;

    sf::View UIView;

    UIManagementSystem uiManagementSystem;

    std::map<unsigned int, std::unique_ptr<UIElement>> m_elements;
};