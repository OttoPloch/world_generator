#include "position_component.hpp"
#include "entity_component.hpp"

PositionComponent::PositionComponent(Entity* myEntity, GamePosition position) : EntityComponent(myEntity), position(position) {}