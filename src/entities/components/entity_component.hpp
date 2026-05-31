#pragma once

#include "../../core/common.hpp"

class Entity;

struct EntityComponent
{
    EntityComponent(Entity* myEntity);

    virtual ~EntityComponent();

    Entity* myEntity;
};