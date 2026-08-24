#pragma once

#include "components/entity_component.hpp"

class Game;

class Entity
{
public:
    Entity(int ID, Game* game);

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
    T* addComponent(Args&&... args)
    {
        T* comp = new T(std::forward<Args>(args)...);
        components.emplace_back(comp);
        return comp;
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
    
    const int ID;

    Game* game;
private:
    std::vector<std::unique_ptr<EntityComponent>> components;
};