#pragma once

#include "../../core/common.hpp"
#include "entity_component.hpp"

struct ItemComponent : EntityComponent
{
    ItemComponent(Entity* myEntity);
};