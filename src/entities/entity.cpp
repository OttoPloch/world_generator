#include "entity.hpp"
#include "components/entity_component.hpp"

Entity::Entity(unsigned int ID, Game* game, sf::Vector2f position) : ID(ID), game(game), position(game, position) {}

std::vector<EntityComponent*> Entity::getComponentsOfType(std::string componentTypeIdentifier)
{
    std::vector<EntityComponent*> componentsOfType;

    for (auto& c : components)
    {
        if (c->componentTypeIdentifier == componentTypeIdentifier)
        {
            componentsOfType.emplace_back(c.get());
        }
    }

    return componentsOfType;
}