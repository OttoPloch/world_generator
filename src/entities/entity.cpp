#include "entity.hpp"

Entity::Entity(int ID, Game* game, sf::Vector2f position) : ID(ID), game(game), position(game, position) {}

EntityComponent* Entity::getComponent(std::string componentTypeName)
{
    for (auto& c : components)
    {
        if (c->componentTypeName == componentTypeName)
        {
            return c.get();
        }
    }

    return nullptr;
}