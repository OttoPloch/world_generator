#pragma once

#include "../../core/common.hpp"
#include "entity_component.hpp"

struct ItemComponent : EntityComponent
{
    ItemComponent(Entity* myEntity, sf::Vector2f spawnAreaOffset, sf::Vector2f spawnAreaSize);

    void init(std::string resource, unsigned int amount);

    std::string resource;
    unsigned int amount;
};