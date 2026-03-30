#pragma once

#include "common.hpp"

class Entity;

struct EntityComponent
{
    EntityComponent(Entity* myEntity);

    virtual ~EntityComponent();
    
    virtual void tick();

    virtual void update();

    Entity* myEntity;
};