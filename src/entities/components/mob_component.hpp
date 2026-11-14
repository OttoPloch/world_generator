#pragma once

#include "../../core/common.hpp"
#include "entity_component.hpp"
#include "component_data.hpp"

struct MobComponent : public EntityComponent
{
    MobComponent(Entity* p_myEntity, MobComponentData p_stats);

    MobComponentData m_stats;
};