#pragma once

#include "../core/common.hpp"
#include "ui_position.hpp"
#include "components/ui_component.hpp"
#include "components/text_component.hpp"
#include "components/background_component.hpp"

class UIElement
{
public:
    UIElement(Game* game, std::string name, UIPosition position, int z);

    // total bounding rect including any components that go outside the element.
    sf::FloatRect getGlobalBounds();
    
    void updateVisuals();

    void draw();

    template<typename T>
    T* getComponent()
    {
        for (auto& c : components)
        {
            if (auto casted = dynamic_cast<T*>(c.get())) return casted;
        }
        
        return nullptr;
    }

    template<typename T, typename... Args>
    T& addComponent(Args&&... args)
    {
        T* comp = new T(std::forward<Args>(args)...);
        components.emplace_back(comp);
        return *comp;
    }

    template<typename T>
    void removeComponent()
    {
        for (auto c = components.begin(); c != components.end();)
        {
            if (auto casted = dynamic_cast<T*>(c->get()))
            {
                c = components.erase(c);
                break;
            }
            else
            {
                c++;
            }
        }
    }

    Game* game;
    std::string name;
    UIPosition position;
    int z;

    std::vector<std::unique_ptr<UIComponent>> components;
};