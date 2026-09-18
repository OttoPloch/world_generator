#pragma once

#include "../../core/common.hpp"

class Entity;

struct EntityComponent
{
    EntityComponent(Entity* myEntity, std::string componentTypeName);

    virtual ~EntityComponent();

    Entity* myEntity;

    std::string componentTypeName;
};