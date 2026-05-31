#include "entity_component.hpp"
#include "../entity.hpp"

EntityComponent::EntityComponent(Entity* myEntity) : myEntity(myEntity) {}

EntityComponent::~EntityComponent() {}