#pragma once

#include "entity_component.hpp"

class Entity;

struct ControlComponent : public EntityComponent
{
    ControlComponent(Entity* myEntity);
};