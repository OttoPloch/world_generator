#pragma once

#include "entity_component.hpp"

struct ControlComponent : public EntityComponent
{
    ControlComponent(Entity* myEntity);
};