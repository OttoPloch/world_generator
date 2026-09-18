#include "entity_component.hpp"
#include "../entity.hpp"

EntityComponent::EntityComponent(Entity* myEntity, std::string componentTypeName) : myEntity(myEntity), componentTypeName(componentTypeName) {}

EntityComponent::~EntityComponent() {}