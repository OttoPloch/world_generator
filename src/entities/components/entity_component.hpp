#pragma once

#include "../../core/common.hpp"

class Entity;

struct EntityComponent
{
    EntityComponent(Entity* myEntity);

    virtual ~EntityComponent();
    
    virtual void tick();

    virtual void update(float dt);

    Entity* myEntity;
};