#pragma once

#include "../core/common.hpp"
#include "ui_position.hpp"
#include "components/ui_component.hpp"
#include "components/text_component.hpp"
#include "components/background_component.hpp"
#include <SFML/Window/Mouse.hpp>

class UIElement
{
public:
    UIElement(Game* game, std::string name, UIPosition position, int z = 0, UIElement* parent = nullptr);

    // total bounding rect including any components that go outside the element.
    sf::FloatRect getGlobalBounds();
    
    std::vector<sf::FloatRect> getAllComponentBounds();

    // checks if the component given has the highest or equal to the highest sortIndex
    // at the position given. Uses GLOBAL coordinates, not local. Returns false if point
    // does not collide with the given component. Designed to be used for registering presses.
    bool isComponentOnTopAtPoint(UIComponent* component, sf::Vector2f point);

    // returns the local bounds of the element, including every component UP TO
    // those with the sortIndex given. This is designed to be used by UI components
    // to get the relative space for their anchors.
    sf::FloatRect getLocalBoundsUpToComponent(int sortIndex);

    void updateVisuals();

    void update(float dt);

    void draw(bool debug);

    template<typename T>
    T* getComponent(std::string identifier = "")
    {
        for (auto& c : components)
        {
            if (auto casted = dynamic_cast<T*>(c.get()))
            {
                if (identifier == "")
                {
                    return casted;
                }
                else
                {
                    if (c->identifier == identifier) return casted;
                }
            }

        }
        
        return nullptr;
    }

    template<typename T, typename... Args>
    T& addComponent(Args&&... args)
    {
        T* comp = new T(std::forward<Args>(args)...);
        components.emplace_back(comp);
        sortComponents();
        return *comp;
    }

    template<typename T>
    void removeComponent(std::string identifier = "")
    {
        for (auto c = components.begin(); c != components.end();)
        {
            if (auto casted = dynamic_cast<T*>(c->get()))
            {
                if (identifier == "")
                {
                    c = components.erase(c);
                    sortComponents();
                    break;
                }
                else
                {
                    if (c->get()->identifier == identifier)
                    {
                        c = components.erase(c);
                        sortComponents();
                        break;
                    }
                }
            }
            else
            {
                c++;
            }
        }
    }

    Game* game;
    std::string name;
    // the position is basically an offset, and in order to
    // find the effective position, this value must be added
    // to the position of this element's parent, and it's parent,
    // and so on and so forth until the final parent, whose position
    // is an offset from 0,0 (just a normal position). This is the
    // purpose of the getEffectivePosition() method.
    UIPosition position;
    int z;
    
    sf::Vector2f anchorOffset;
    sf::Vector2f effectivePosition;

    UIElement* parent;
    
    std::vector<std::unique_ptr<UIComponent>> components;
private:
    sf::Vector2f calculateEffectivePosition();
    
    void sortComponents();
};