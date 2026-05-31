#pragma once

#include "entity_component.hpp"
#include "../states.hpp"

class Entity;

struct StateComponent : public EntityComponent
{
    StateComponent(Entity* myEntity);
};