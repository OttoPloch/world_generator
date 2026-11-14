#include "mob_component.hpp"
#include "component_data.hpp"
#include "../entity.hpp"
#include "../../core/game.hpp"

MobComponent::MobComponent(Entity* p_myEntity, MobComponentData p_stats) : EntityComponent(p_myEntity, "mob"), m_stats(p_stats)
{
    m_moveTimer = m_stats.ticksToMove;
}