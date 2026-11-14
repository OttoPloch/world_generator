#include "mob_component.hpp"
#include "component_data.hpp"

MobComponent::MobComponent(Entity* p_myEntity, MobComponentData p_stats) : EntityComponent(p_myEntity, "mob"), m_stats(p_stats) {}